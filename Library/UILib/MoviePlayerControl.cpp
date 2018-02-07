#include "MoviePlayerControl.h"
#include "ControlManager.h"
#include "WindowControl.h"

#include "BaseLib\ConsoleOutput.h"
#include "CommonLib\GameManager.h"


#include <thread>

namespace jojogame {
void CMoviePlayerControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CMoviePlayerControl, "_MoviePlayer");

    LUA_METHOD(GetX);
    LUA_METHOD(GetY);
    LUA_METHOD(GetWidth);
    LUA_METHOD(GetHeight);
    LUA_METHOD(IsPlaying);
    LUA_METHOD(SetX);
    LUA_METHOD(SetY);
    LUA_METHOD(SetWidth);
    LUA_METHOD(SetHeight);
    LUA_METHOD(SetEndEvent);

    LUA_METHOD(Play);
    LUA_METHOD(WaitForPlay);
    LUA_METHOD(Stop);
    LUA_METHOD(Create);
    LUA_METHOD(Destroy);
}

CMoviePlayerControl::CMoviePlayerControl(CWindowControl *parent, std::wstring fileName)
{
    if (parent)
    {
        _parent = parent;
    }
    _fileName = fileName;
}

CMoviePlayerControl::~CMoviePlayerControl()
{
    CMoviePlayerControl::Destroy();
}

bool CMoviePlayerControl::IsPlaying()
{
    return _playing;
}

std::wstring CMoviePlayerControl::GetEndEvent()
{
    return _endEvent;
}

void CMoviePlayerControl::SetEndEvent(std::wstring endEvent)
{
    _endEvent = endEvent;
}

void CMoviePlayerControl::SetFileName(std::wstring fileName)
{
    _fileName = fileName;
}


void packet_queue_init(PacketQueue *q)
{
    memset(q, 0, sizeof(PacketQueue));
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}

int packet_queue_put(PacketQueue *q, AVPacket *pkt)
{

    AVPacketList *pkt1;
    pkt1 = (AVPacketList *) av_malloc(sizeof(AVPacketList));
    if (!pkt1)
    {
        return AVERROR(ENOMEM);
    }
    if (av_packet_ref(&pkt1->pkt, pkt) < 0)
    {
        return -1;
    }
    pkt1->next = NULL;


    SDL_LockMutex(q->mutex);

    if (!q->last_pkt)
    {
        q->first_pkt = pkt1;
    } else
    {
        q->last_pkt->next = pkt1;
    }
    q->last_pkt = pkt1;
    q->nb_packets++;
    q->size += pkt1->pkt.size;
    SDL_CondSignal(q->cond);

    SDL_UnlockMutex(q->mutex);
    return 0;
}

static int packet_queue_get(CMoviePlayerControl *movieControl, AVPacket *pkt, int block)
{
    auto q = movieControl->GetPointerAudioQueue();
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for (;;)
    {

        if (!movieControl->IsPlaying())
        {
            ret = -1;
            break;
        }

        pkt1 = q->first_pkt;
        if (pkt1)
        {
            q->first_pkt = pkt1->next;
            if (!q->first_pkt)
            {
                q->last_pkt = NULL;
            }
            q->nb_packets--;
            q->size -= pkt1->pkt.size;
            *pkt = pkt1->pkt;
            av_free(pkt1);
            ret = 1;
            break;
        } else if (!block)
        {
            ret = 0;
            break;
        } else
        {
            SDL_CondWait(q->cond, q->mutex);
        }
    }
    SDL_UnlockMutex(q->mutex);
    return ret;
}

bool CMoviePlayerControl::Create()
{
    AVCodecParameters *videoCodecParameters = nullptr;
    AVCodecParameters *audioCodecParameters = nullptr;
    AVCodec *videoCodec;
    AVCodec *audioCodec;
    int error = 0;

    int length;
    length = WideCharToMultiByte(CP_ACP, 0, _fileName.c_str(), -1, nullptr, NULL, nullptr, nullptr);
    auto *str = new char[length];
    WideCharToMultiByte(CP_ACP, 0, _fileName.c_str(), -1, str, length, nullptr, nullptr);
    error = avformat_open_input(&_formatContext, str, nullptr, nullptr);
    delete[] str;
    if (error < 0)
    {
        CConsoleOutput::OutputConsoles(L"File open error");
        return false;
    }

    if (avformat_find_stream_info(_formatContext, nullptr) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find stream information");
        return false;
    }

    // Find video stream
    for (unsigned int i = 0; i < _formatContext->nb_streams; i++)
    {
        if (_formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO && videoCodecParameters == nullptr)
        {
            videoCodecParameters = _formatContext->streams[i]->codecpar;
            _videoStreamIndex = i;
        }
        if (_formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO && audioCodecParameters == nullptr)
        {
            audioCodecParameters = _formatContext->streams[i]->codecpar;
            _audioStreamIndex = i;
        }
    }
    if (videoCodecParameters == nullptr || audioCodecParameters == nullptr)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find video or audio stream");
        return false;
    }

    videoCodec = avcodec_find_decoder(videoCodecParameters->codec_id);
    if (videoCodec == nullptr)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find video decoder");
        return false;
    }
    audioCodec = avcodec_find_decoder(audioCodecParameters->codec_id);
    if (audioCodec == nullptr)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find audio decoder");
        return false;
    }

    _videoCodecContext = avcodec_alloc_context3(videoCodec);
    if (avcodec_parameters_to_context(_videoCodecContext, videoCodecParameters) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Failed to convert to context");
        return false;
    }
    _audioCodecContext = avcodec_alloc_context3(audioCodec);
    if (avcodec_parameters_to_context(_audioCodecContext, audioCodecParameters) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Failed to convert to context");
        return false;
    }

    if (avcodec_open2(_videoCodecContext, videoCodec, nullptr) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot open codec");
        return false;
    }
    if (avcodec_open2(_audioCodecContext, audioCodec, nullptr) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot open codec");
        return false;
    }

    packet_queue_init(&_audioQueue);
    _fps = av_q2d(_formatContext->streams[_videoStreamIndex]->avg_frame_rate);

    _position.x = _position.y = 0;
    _size.cx = _videoCodecContext->width;
    _size.cy = _videoCodecContext->height;

    return true;
}

void CMoviePlayerControl::Destroy()
{
    Stop();
    if (_formatContext)
    {
        avformat_close_input(&_formatContext);
        _formatContext = nullptr;
    }

    if (_videoCodecContext)
    {
        avcodec_close(_videoCodecContext);
        _videoCodecContext = nullptr;
    }
}


int audio_decode_frame(CMoviePlayerControl *movieControl, uint8_t *audio_buf, int buf_size)
{
    auto audioContext = movieControl->GetAudioCodecContext();
    static AVPacket pkt;
    static uint8_t *audio_pkt_data = nullptr;
    static int audio_pkt_size = 0;
    static AVFrame frame;

    int packetError, data_size = 0;

    for (;;)
    {
        while (audio_pkt_size > 0)
        {
            packetError = avcodec_send_packet(audioContext, &pkt);
            char err[4096];
            av_make_error_string(err, 4096, packetError);
            if (packetError < 0)
            {
                /* if error, skip frame */
                audio_pkt_size = 0;
                break;
            }
            audio_pkt_data += pkt.size;
            audio_pkt_size -= pkt.size;
            data_size = 0;
            if (avcodec_receive_frame(audioContext, &frame) == 0)
            {
                data_size = av_samples_get_buffer_size(nullptr,
                                                       audioContext->channels,
                                                       frame.nb_samples,
                                                       audioContext->sample_fmt,
                                                       1);
                memcpy(audio_buf, frame.data[0], static_cast<size_t>(data_size));
            }
            if (data_size <= 0)
            {
                /* No data yet, get more frames */
                continue;
            }
            /* We have data, return it and come back for more later */
            return data_size;
        }
        if (pkt.data)
        {
            av_packet_unref(&pkt);
        }

        if (!movieControl->IsPlaying())
        {
            return -1;
        }

        if (packet_queue_get(movieControl, &pkt, 1) < 0)
        {
            return -1;
        }
        audio_pkt_data = pkt.data;
        audio_pkt_size = pkt.size;
    }
}

void audioCallback(void *userdata, Uint8 *stream, int len)
{
    CMoviePlayerControl *moviePlayerControl = (CMoviePlayerControl *) userdata;
    int len1, audio_size;

    static uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    static unsigned int audio_buf_size = 0;
    static unsigned int audio_buf_index = 0;

    while (len > 0)
    {
        if (audio_buf_index >= audio_buf_size)
        {
            /* We have already sent all our data; get more */
            audio_size = audio_decode_frame(moviePlayerControl, audio_buf, sizeof(audio_buf));
            if (audio_size < 0)
            {
                /* If error, output silence */
                audio_buf_size = 1024; // arbitrary?
                memset(audio_buf, 0, audio_buf_size);
            } else
            {
                audio_buf_size = static_cast<unsigned int>(audio_size);
            }
            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;
        if (len1 > len)
        {
            len1 = len;
        }
        memcpy(stream, (uint8_t *) audio_buf + audio_buf_index, len1);
        len -= len1;
        stream += len1;
        audio_buf_index += len1;
    }
}

bool PlayMovie(CMoviePlayerControl *movieControl, SwsContext *imageConvertContext)
{
    SDL_Event event;
    AVCodecContext *codecContext = movieControl->GetVideoCodecContext();
    HDC dc = GetDC(movieControl->GetParentControl()->GetHWnd());
    HDC newDC = CreateCompatibleDC(dc);

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 32);
    if (numBytes < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot get image buffer size");
        return false;
    }

    auto buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    AVFrame *frame = av_frame_alloc();
    AVFrame *frameBGR = av_frame_alloc();
    av_image_fill_arrays(frameBGR->data, frameBGR->linesize, buffer, AV_PIX_FMT_RGB24,
                         codecContext->width, codecContext->height, 32);

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biHeight = -codecContext->height;
    bmi.bmiHeader.biWidth = codecContext->width;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biSizeImage = static_cast<DWORD>(codecContext->height * codecContext->width * 3);
    auto newBitmap = CreateDIBSection(newDC, &bmi, DIB_RGB_COLORS, (void **) &buffer, NULL, 0); //&pbmpdata
    HBITMAP oldBitmap = (HBITMAP) SelectObject(newDC, newBitmap);

    AVPacket packet{};
    av_init_packet(&packet);
    while (movieControl->IsPlaying() && (av_read_frame(movieControl->GetAVFormatContext(), &packet) >= 0))
    {
        // Is this a packet from the video stream?
        if (packet.stream_index == movieControl->GetVideoStreamIndex())
        {
            /// Decode video frame
            if (avcodec_send_packet(codecContext, &packet) == 0 && avcodec_receive_frame(codecContext, frame) == 0)
            {
                sws_scale(imageConvertContext, (uint8_t const *const *) frame->data,
                          frame->linesize, 0, codecContext->height,
                          frameBGR->data, frameBGR->linesize);

                frameBGR->data[0] = buffer;
                frameBGR->linesize[0] = codecContext->width * 3;

                if (movieControl->IsPlaying())
                {
                    BitBlt(dc, 0, 0, codecContext->width, codecContext->height, newDC, 0, 0, SRCCOPY);
                    CGameManager::GetInstance().Delay(1000.0 / movieControl->GetFps());
                }
                av_packet_unref(&packet);
            }
        } else if (packet.stream_index == movieControl->GetAudioStreamIndex())
        {
           packet_queue_put(movieControl->GetPointerAudioQueue(), &packet);
        } else
        {
            av_packet_unref(&packet);
        }

        SDL_PollEvent(&event);
        switch (event.type)
        {
            case SDL_QUIT:
                SDL_Quit();
                break;
            default:
                break;
        }
    }

    // Free the packet that was allocated by av_read_frame

    SelectObject(newDC, oldBitmap);
    DeleteBitmap(newBitmap);
    av_free(frameBGR);
    av_free(frame);

    DeleteDC(newDC);
    ReleaseDC(movieControl->GetParentControl()->GetHWnd(), dc);
}

bool PlaySound(CMoviePlayerControl *movieControl)
{
    SDL_AudioSpec audioSpec;
    SDL_AudioSpec spec;
    auto audioContext = movieControl->GetAudioCodecContext();
    audioSpec.freq = audioContext->sample_rate;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = audioContext->channels;
    audioSpec.silence = 0;
    audioSpec.samples = 1024;
    audioSpec.callback = audioCallback;
    audioSpec.userdata = movieControl;

    if (SDL_OpenAudio(&audioSpec, NULL))
    {
        return false;
    }

    SDL_PauseAudio(0);
}

void CMoviePlayerControl::Play()
{
    std::thread t([&]()
                  {
                      _playing = true;

                      PlaySound(this);

                      int width = _videoCodecContext->width;
                      int height = _videoCodecContext->height;
                      SwsContext *imageConvertContext = nullptr;
                      imageConvertContext = sws_getCachedContext(imageConvertContext, width, height,
                                                                 _videoCodecContext->pix_fmt,
                                                                 width, height, AV_PIX_FMT_BGR24,
                                                                 SWS_BICUBIC, nullptr, nullptr, nullptr);

                      PlayMovie(this, imageConvertContext);
                      _playing = false;
                  });
    t.detach();

}

void CMoviePlayerControl::WaitForPlay()
{
    auto quit = false;
    MSG message;

    for (;;)
    {
        if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT)
            {
                quit = true;
                break;
            }

            TranslateMessage(&message);
            DispatchMessage(&message);
        } else if (!_playing)
        {
            break;
        }
    }

    if (quit)
    {
        PostQuitMessage(0);
    }
}

void CMoviePlayerControl::Stop()
{
    if (_playing)
    {
        _playing = false;
    }
}

int CMoviePlayerControl::GetX()
{
    return _position.x;
}

int CMoviePlayerControl::GetY()
{
    return _position.y;
}

int CMoviePlayerControl::GetWidth()
{
    return _size.cx;
}

int CMoviePlayerControl::GetHeight()
{
    return _size.cy;
}

CWindowControl *CMoviePlayerControl::GetParentControl()
{
    return _parent;
}

int CMoviePlayerControl::GetDrawingIndex()
{
    return _drawingIndex;
}

AVFormatContext *CMoviePlayerControl::GetAVFormatContext()
{
    return _formatContext;
}

AVCodecContext *CMoviePlayerControl::GetVideoCodecContext()
{
    return _videoCodecContext;
}

AVCodecContext *CMoviePlayerControl::GetAudioCodecContext()
{
    return _audioCodecContext;
}

int CMoviePlayerControl::GetVideoStreamIndex()
{
    return _videoStreamIndex;
}

int CMoviePlayerControl::GetAudioStreamIndex()
{
    return _audioStreamIndex;
}

double CMoviePlayerControl::GetFps()
{
    return _fps;
}

void CMoviePlayerControl::SetX(int x)
{
    _position.x = x;
}

void CMoviePlayerControl::SetY(int y)
{
    _position.y = y;
}

void CMoviePlayerControl::SetWidth(int width)
{
    _size.cx = width;
}

void CMoviePlayerControl::SetHeight(int height)
{
    _size.cy = height;
}

void CMoviePlayerControl::SetDrawingIndex(int index)
{
    _drawingIndex = index;
}

PacketQueue *CMoviePlayerControl::GetPointerAudioQueue()
{
    return &_audioQueue;
}
}