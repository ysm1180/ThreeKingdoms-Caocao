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

CMoviePlayerControl::CMoviePlayerControl(CWindowControl *parent, std::string fileName)
{
    if (parent)
    {
        _parent = parent;
        _state.parentControlHWnd = parent->GetHWnd();
    }
    _state.fileName = fileName;
}

CMoviePlayerControl::~CMoviePlayerControl()
{
    CMoviePlayerControl::Destroy();
}

bool CMoviePlayerControl::IsPlaying()
{
    return _state.playing;
}

std::wstring CMoviePlayerControl::GetEndEvent()
{
    return _endEvent;
}

void CMoviePlayerControl::SetEndEvent(std::wstring endEvent)
{
    _endEvent = endEvent;
}

void CMoviePlayerControl::SetFileName(std::string fileName)
{
    _state.fileName = fileName;
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

    av_init_packet(&pkt1->pkt);
    if (av_packet_ref(&pkt1->pkt, pkt) < 0) {
        return AVERROR(ENOMEM);
    }
    //pkt1->pkt = *pkt;
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

static int packet_queue_get(VideoState *videoState, PacketQueue *q, AVPacket *pkt, int block)
{
    AVPacketList *pkt1;
    int ret;

    SDL_LockMutex(q->mutex);

    for (;;)
    {
        if (videoState->playing == false)
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

int audio_decode_frame(VideoState *videoState, uint8_t *audio_buf, int buf_size)
{
    int packetError, data_size = 0;
    AVPacket *pkt = &videoState->audioPacket;

    for (;;)
    {
        while (videoState->audioPacketSize > 0)
        {
            packetError = avcodec_send_packet(videoState->audioCodecContext, pkt);
            if (pkt->size < 0)
            {
                /* if error, skip frame */
                videoState->audioPacketSize = 0;
                break;
            }
            data_size = 0;
            if (avcodec_receive_frame(videoState->audioCodecContext, &videoState->audioFrame) == 0)
            {
                data_size = av_samples_get_buffer_size(nullptr,
                                                       videoState->audioCodecContext->channels,
                                                       videoState->audioFrame.nb_samples,
                                                       videoState->audioCodecContext->sample_fmt,
                                                       1);
                memcpy(audio_buf, videoState->audioFrame.data[0], static_cast<size_t>(data_size));
            }

            videoState->audioPacketData += pkt->size;
            videoState->audioPacketSize -= pkt->size;

            if (data_size <= 0)
            {
                /* No data yet, get more frames */
                continue;
            }
            /* We have data, return it and come back for more later */
            return data_size;
        }

        if (pkt->data)
        {
            av_packet_unref(pkt);
        }

        if (videoState->playing == false)
        {
            return -1;
        }

        if (packet_queue_get(videoState, &videoState->audioQueue, pkt, 1) < 0)
        {
            return -1;
        }

        videoState->audioPacketData = pkt->data;
        videoState->audioPacketSize = pkt->size;
    }
}

void audioCallback(void *userdata, Uint8 *stream, int len)
{
    auto videoState = (VideoState *) userdata;
    int len1, audio_size;

    while (len > 0)
    {
        if (videoState->audioBufferIndex >= videoState->audioBufferSize)
        {
            /* We have already sent all our data; get more */
            audio_size = audio_decode_frame(videoState, videoState->audioBuffer, sizeof(videoState->audioBuffer));
            if (audio_size < 0)
            {
                /* If error, output silence */
                videoState->audioBufferSize = 1024; // arbitrary?
                memset(videoState->audioBuffer, 0, videoState->audioBufferSize);
            } else
            {
                videoState->audioBufferSize = static_cast<unsigned int>(audio_size);
            }
            videoState->audioBufferIndex = 0;
        }
        len1 = videoState->audioBufferSize - videoState->audioBufferIndex;
        if (len1 > len)
        {
            len1 = len;
        }
        memcpy(stream, (uint8_t *) videoState->audioBuffer + videoState->audioBufferIndex, len1);
        len -= len1;
        stream += len1;
        videoState->audioBufferIndex += len1;
    }
}


static Uint32 sdl_refresh_timer_cb(Uint32 interval, void *opaque)
{
    SDL_Event event;
    event.type = FF_REFRESH_EVENT;
    event.user.data1 = opaque;
    SDL_PushEvent(&event);
    return 0; /* 0 means stop timer */
}

/* schedule a video refresh in 'delay' ms */
static void schedule_refresh(VideoState *is, int delay)
{
    SDL_AddTimer(delay, sdl_refresh_timer_cb, is);
}

void DisplayVideo(VideoState *videoState)
{
    VideoFrame *videoFrame;
    float aspect_ratio;

    videoFrame = &videoState->frameQueue[videoState->frameQueueRearIndex];
    if (videoFrame->dc)
    {
//        if (videoState->videoCodecContext->sample_aspect_ratio.num == 0)
//        {
//            aspect_ratio = 0;
//        } else
//        {
//            aspect_ratio = av_q2d(videoState->videoCodecContext->sample_aspect_ratio) *
//                           videoState->videoCodecContext->width / videoState->videoCodecContext->height;
//        }
//        if (aspect_ratio <= 0.0)
//        {
//            aspect_ratio = (float) videoState->videoCodecContext->width /
//                           (float) videoState->videoCodecContext->height;
//        }

        SDL_LockMutex(videoState->screenMutex);

        if (videoState->playing)
        {
            HDC dc = GetDC(videoState->parentControlHWnd);
            BitBlt(dc, videoState->position.x, videoState->position.y, videoState->videoCodecContext->width,
                videoState->videoCodecContext->height, videoFrame->dc, 0, 0, SRCCOPY);
            ReleaseDC(videoState->parentControlHWnd, dc);
        }

        SDL_UnlockMutex(videoState->screenMutex);

    }
}

void video_refresh_timer(void *userdata)
{

    VideoState *videoState = (VideoState *) userdata;
    VideoFrame *videoFrame;

    if (videoState->videoStream)
    {
        if (videoState->frameQueueSize == 0)
        {
            schedule_refresh(videoState, 1);
        } else
        {
            videoFrame = &videoState->frameQueue[videoState->frameQueueRearIndex];
            /* Now, normally here goes a ton of code
           about timing, etc. we're just going to
           guess at a delay for now. You can
           increase and decrease this value and hard code
           the timing - but I don't suggest that ;)
           We'll learn how to do it for real later.
            */
            schedule_refresh(videoState, 40);

            DisplayVideo(videoState);

            if (++videoState->frameQueueRearIndex == VIDEO_FRAME_QUEUE_SIZE)
            {
                videoState->frameQueueRearIndex = 0;
            }
            SDL_LockMutex(videoState->frameQueueMutex);
            videoState->frameQueueSize--;
            SDL_CondSignal(videoState->frameQueueCond);
            SDL_UnlockMutex(videoState->frameQueueMutex);
        }
    } else
    {
        schedule_refresh(videoState, 100);
    }
}

void alloc_picture(void *userdata)
{
    VideoState *videoState = (VideoState *) userdata;
    VideoFrame *videoFrame;

    videoFrame = &videoState->frameQueue[videoState->frameQueueWIndex];
    if (videoFrame->dc)
    {
        SelectObject(videoFrame->dc, videoFrame->oldBitmap);
        DeleteBitmap(videoFrame->newBitmap);
        DeleteDC(videoFrame->dc);

    }

    SDL_LockMutex(videoState->screenMutex);

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, videoState->videoCodecContext->width,
                                            videoState->videoCodecContext->height, 32);
    if (numBytes < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot get image buffer size");
        return;
    }
    videoFrame->buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biHeight = -videoState->videoCodecContext->height;
    bmi.bmiHeader.biWidth = videoState->videoCodecContext->width;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biSizeImage = static_cast<DWORD>(videoState->videoCodecContext->height *
                                                   videoState->videoCodecContext->width * 3);

    HDC dc = GetDC(videoState->parentControlHWnd);
    videoFrame->dc = CreateCompatibleDC(dc);
    videoFrame->newBitmap = CreateDIBSection(videoFrame->dc, &bmi, DIB_RGB_COLORS, (void **) &videoFrame->buffer,
                                             nullptr, 0);
    videoFrame->oldBitmap = (HBITMAP) SelectObject(videoFrame->dc, videoFrame->newBitmap);

    ReleaseDC(videoState->parentControlHWnd, dc);

    SDL_UnlockMutex(videoState->screenMutex);

    videoFrame->width = videoState->videoCodecContext->width;
    videoFrame->height = videoState->videoCodecContext->height;
    videoFrame->allocated = 1;

}

int queue_picture(VideoState *videoState, AVFrame *frame)
{
    VideoFrame *videoFrame;

    /* wait until we have space for a new pic */
    SDL_LockMutex(videoState->frameQueueMutex);
    while (videoState->frameQueueSize >= VIDEO_FRAME_QUEUE_SIZE &&
           videoState->playing)
    {
        SDL_CondWait(videoState->frameQueueCond, videoState->frameQueueMutex);
    }
    SDL_UnlockMutex(videoState->frameQueueMutex);

    if (!videoState->playing)
    {
        return -1;
    }

    videoFrame = &videoState->frameQueue[videoState->frameQueueWIndex];

    if (!videoFrame->buffer ||
        videoFrame->width != videoState->videoCodecContext->width ||
        videoFrame->height != videoState->videoCodecContext->height)
    {
        SDL_Event event;

        videoFrame->allocated = 0;
        alloc_picture(videoState);
        if (!videoState->playing)
        {
            return -1;
        }
    }

    if (videoFrame->buffer)
    {
        AVFrame *frameBGR = av_frame_alloc();

        av_image_fill_arrays(frameBGR->data, frameBGR->linesize, videoFrame->buffer, AV_PIX_FMT_RGB24,
                             videoState->videoCodecContext->width, videoState->videoCodecContext->height, 32);

        sws_scale(videoState->swsContext, (uint8_t const *const *) frame->data,
                  frame->linesize, 0, videoState->videoCodecContext->height,
                  frameBGR->data, frameBGR->linesize);

        frameBGR->data[0] = videoFrame->buffer;
        frameBGR->linesize[0] = videoState->videoCodecContext->width * 3;

        if (++videoState->frameQueueWIndex == VIDEO_FRAME_QUEUE_SIZE)
        {
            videoState->frameQueueWIndex = 0;
        }
        SDL_LockMutex(videoState->frameQueueMutex);
        videoState->frameQueueSize++;
        SDL_UnlockMutex(videoState->frameQueueMutex);

        av_free(frameBGR);
    }

    return 0;
}

int video_thread(void *arg)
{
    VideoState *videoState = (VideoState *) arg;
    AVPacket packet;
    AVFrame *frame;

    frame = av_frame_alloc();

    for (;;)
    {
        if (packet_queue_get(videoState, &videoState->videoQueue, &packet, 1) < 0)
        {
            // means we quit getting packets
            break;
        }

        if (avcodec_send_packet(videoState->videoCodecContext, &packet) == 0)
        {
            if (avcodec_receive_frame(videoState->videoCodecContext, frame) == 0)
            {
                if (queue_picture(videoState, frame) < 0)
                {
                    break;
                }
            }
        }

        av_packet_unref(&packet);
    }

    av_frame_free(&frame);
    return 0;
}

bool CMoviePlayerControl::Create()
{
    AVCodecParameters *videoCodecParameters = nullptr;
    AVCodecParameters *audioCodecParameters = nullptr;
    AVCodec *videoCodec;
    AVCodec *audioCodec;
    int error = 0;

    _state.screenMutex = SDL_CreateMutex();
    _state.frameQueueMutex = SDL_CreateMutex();
    _state.frameQueueCond = SDL_CreateCond();

    int length;
    _state.formatContext = nullptr;
    error = avformat_open_input(&_state.formatContext, _state.fileName.c_str(), nullptr, nullptr);
    if (error < 0)
    {
        CConsoleOutput::OutputConsoles(L"File open error");
        return false;
    }

    if (avformat_find_stream_info(_state.formatContext, nullptr) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find stream information");
        return false;
    }

    // Find video stream
    for (unsigned int i = 0; i < _state.formatContext->nb_streams; i++)
    {
        if (_state.formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO &&
            videoCodecParameters == nullptr)
        {
            videoCodecParameters = _state.formatContext->streams[i]->codecpar;
            _state.videoStream = _state.formatContext->streams[i];
            _state.videoStreamIndex = i;
        }
        if (_state.formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO &&
            audioCodecParameters == nullptr)
        {
            audioCodecParameters = _state.formatContext->streams[i]->codecpar;
            _state.audioStream = _state.formatContext->streams[i];
            _state.audioStreamIndex = i;
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

    _state.videoCodecContext = avcodec_alloc_context3(videoCodec);
    if (avcodec_parameters_to_context(_state.videoCodecContext, videoCodecParameters) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Failed to convert to context");
        return false;
    }
    _state.audioCodecContext = avcodec_alloc_context3(audioCodec);
    if (avcodec_parameters_to_context(_state.audioCodecContext, audioCodecParameters) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Failed to convert to context");
        return false;
    }

    if (avcodec_open2(_state.videoCodecContext, videoCodec, nullptr) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot open codec");
        return false;
    }
    if (avcodec_open2(_state.audioCodecContext, audioCodec, nullptr) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot open codec");
        return false;
    }

    _state.audioBufferSize = 0;
    _state.audioBufferIndex = 0;
    memset(&_state.audioPacket, 0, sizeof(_state.audioPacket));
    packet_queue_init(&_state.videoQueue);
    packet_queue_init(&_state.audioQueue);
    _fps = av_q2d(_state.formatContext->streams[_state.videoStreamIndex]->avg_frame_rate);

    int width = _state.videoCodecContext->width;
    int height = _state.videoCodecContext->height;
    _state.swsContext = nullptr;
    _state.swsContext = sws_getCachedContext(_state.swsContext, width, height,
                                             _state.videoCodecContext->pix_fmt,
                                             width, height, AV_PIX_FMT_BGR24,
                                             SWS_BICUBIC, nullptr, nullptr, nullptr);

    _state.position.x = _state.position.y = 0;
    _state.frameQueueRearIndex = 0;
    _state.frameQueueWIndex = 0;
    _state.frameQueueSize = 0;
    _state.audioBufferSize = 0;
    _state.audioBufferIndex = 0;
    _state.audioPacketData = nullptr;
    _state.audioPacketSize = 0;
    memset(&_state.audioFrame, 0, sizeof(_state.audioFrame));

    _size.cx = _state.videoCodecContext->width;
    _size.cy = _state.videoCodecContext->height;

    return true;
}

void CMoviePlayerControl::Destroy()
{
    Stop();
    if (_state.formatContext)
    {
        avformat_close_input(&_state.formatContext);
        _state.formatContext = nullptr;
    }

    if (_state.videoCodecContext)
    {
        avcodec_close(_state.videoCodecContext);
        _state.videoCodecContext = nullptr;
    }
}

bool PlayMovie(VideoState *videoState)
{
    AVPacket packet;
    videoState->videoThread = SDL_CreateThread(video_thread, "video_thread", videoState);

    for (;;)
    {
        if (videoState->playing == false)
        {
            break;
        }
        // seek stuff goes here
        if (videoState->audioQueue.size > MAX_AUDIOQ_SIZE ||
            videoState->videoQueue.size > MAX_VIDEOQ_SIZE)
        {
            SDL_Delay(10);
            continue;
        }

        if (av_read_frame(videoState->formatContext, &packet) < 0)
        {
            if (videoState->formatContext->pb->error == 0)
            {
                SDL_Delay(100); /* no error; wait for user input */
                continue;
            } else
            {
                break;
            }
        }

        if (packet.stream_index == videoState->videoStreamIndex)
        {
            packet_queue_put(&videoState->videoQueue, &packet);
        } else if (packet.stream_index == videoState->audioStreamIndex)
        {
            packet_queue_put(&videoState->audioQueue, &packet);
        } else
        {
            av_packet_unref(&packet);
        }
    }

    while (videoState->playing)
    {
        SDL_Delay(100);
    }


    SDL_Event event;
    event.type = FF_QUIT_EVENT;
    event.user.data1 = videoState;
    SDL_PushEvent(&event);

    return true;
}

bool PlaySound(VideoState *videoState)
{
    SDL_AudioSpec audioSpec;
    SDL_AudioSpec spec;
    auto audioContext = videoState->audioCodecContext;
    audioSpec.freq = audioContext->sample_rate;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = audioContext->channels;
    audioSpec.silence = 0;
    audioSpec.samples = 8192;
    audioSpec.callback = audioCallback;
    audioSpec.userdata = videoState;

    if (SDL_OpenAudio(&audioSpec, NULL))
    {
        return false;
    }

    SDL_PauseAudio(0);

    return true;
}


int decode_thread(void *arg)
{
    VideoState *is = (VideoState *)arg;
    PlaySound(is);
    PlayMovie(is);

    return 0;
}

void CMoviePlayerControl::Play()
{
    SDL_Event event;

    schedule_refresh(&_state, 40);

    _state.playing = true;

    _state.parseThread = SDL_CreateThread(decode_thread, "decode_thread", &_state);

    for (; _state.playing;)
    {
        SDL_WaitEvent(&event);
        switch (event.type)
        {
            case FF_QUIT_EVENT:
                _state.playing = false;
                break;
            case SDL_QUIT:
                SDL_Quit();
                PostQuitMessage(0);
                break;
            case FF_REFRESH_EVENT:
                video_refresh_timer(event.user.data1);
                break;
            default:
                break;
        }
    }
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
        } else if (!_state.playing)
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
    if (_state.playing)
    {
        _state.playing = false;
    }
}

int CMoviePlayerControl::GetX()
{
    return _state.position.x;
}

int CMoviePlayerControl::GetY()
{
    return _state.position.y;
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

VideoState *CMoviePlayerControl::GetVideoState()
{
    return &_state;
}

int CMoviePlayerControl::GetDrawingIndex()
{
    return _drawingIndex;
}

double CMoviePlayerControl::GetFps()
{
    return _fps;
}

void CMoviePlayerControl::SetX(int x)
{
    _state.position.x = x;
}

void CMoviePlayerControl::SetY(int y)
{
    _state.position.y = y;
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

}