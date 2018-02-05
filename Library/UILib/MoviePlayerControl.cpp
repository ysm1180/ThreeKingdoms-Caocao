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

bool CMoviePlayerControl::Create()
{
    avcodec_register_all();

    AVFormatContext *formatContext = nullptr;

    int length = WideCharToMultiByte(CP_ACP, 0, _fileName.c_str(), -1, NULL, 0, NULL, NULL);
    char *str = new char[length];
    WideCharToMultiByte(CP_ACP, 0, _fileName.c_str(), -1, str, length, 0, 0);
    int error = avformat_open_input(&formatContext, "C:\\LOGO.avi", NULL, 0);
    if (error != 0)
    {
        delete[] str;
        return false;
    }
    delete[] str;

    if (avformat_find_stream_info(formatContext, NULL) < 0)
    {
        return false;
    }

    int i;
    AVCodecParameters *pCodecCtx = NULL;
    AVCodec *codec;
    // Find the first video stream
    int videoStream = -1;
    for (i = 0; i < formatContext->nb_streams; i++)
    {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoStream = i;
            break;
        }
    }
    if (videoStream == -1)
    {
        return -1;
    } // Didn't find a video stream

    // Get a pointer to the codec context for the video stream
    pCodecCtx = formatContext->streams[videoStream]->codecpar;
    codec = avcodec_find_decoder(pCodecCtx->codec_id);
    auto codecContext = avcodec_alloc_context3(codec);

    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codecContext->width, codecContext->height, 32);
    uint8_t *buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrameRGB = av_frame_alloc();
    av_image_fill_arrays(pFrameRGB->data, pFrameRGB->linesize, buffer, AV_PIX_FMT_RGB24,
                         codecContext->width, codecContext->height, 32);

    HDC dc = GetDC(_parent->GetHWnd());
    HDC newDC = CreateCompatibleDC(dc);


    int w = pCodecCtx->width;
    int h = pCodecCtx->height;
    auto img_convert_ctx = sws_getContext(w, h, codecContext->pix_fmt,
                                          w, h, AV_PIX_FMT_RGB24,
                                          SWS_BICUBIC, NULL, NULL, NULL);

    struct SwsContext *sws_ctx = NULL;
    int frameFinished;
    AVPacket packet;

    
    /*CClientDC dc;*/
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biBitCount = 24;
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biHeight = -codecContext->height;
    bmi.bmiHeader.biWidth = codecContext->width;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biSizeImage = codecContext->height * codecContext->width * 3;
    auto hbmp = CreateDIBSection(newDC, &bmi, DIB_RGB_COLORS, (void **)&buffer, NULL, 0); //&pbmpdata
    av_init_packet(&packet);
    while ((av_read_frame(formatContext, &packet) >= 0))
    {
        // Is this a packet from the video stream?
        if (packet.stream_index == videoStream)
        {
            /// Decode video frame
            //avcodec_decode_video(pCodecCtx, pFrame, &frameFinished,packet.data, packet.size);
            avcodec_send_packet(codecContext, &packet);
            avcodec_receive_frame(codecContext, pFrameRGB);

            // Did we get a video frame?
                sws_scale(img_convert_ctx, (uint8_t const * const *)pFrame->data,
                          pFrame->linesize, 0, codecContext->height,
                          pFrameRGB->data, pFrameRGB->linesize);

                pFrameRGB->data[0] = buffer;
                pFrameRGB->linesize[0] = codecContext->width * 3;

                SelectObject(newDC, hbmp);//hbmp

                BitBlt(dc, 0, 0, codecContext->width, codecContext->height, newDC, 0, 0, SRCCOPY);
                Sleep(10);
        }

        // Free the packet that was allocated by av_read_frame
        av_packet_unref(&packet);
    }

    av_free(pFrameRGB);
    av_free(pFrame);
    av_free(buffer);

    avcodec_close(codecContext);
    avformat_close_input(&formatContext);

    DeleteDC(newDC);
    ReleaseDC(_parent->GetHWnd(), dc);

    /*AVIFileInit();

    int error = AVIFileOpen(&_aviFile, _fileName.c_str(), OF_READ, NULL);
    if (error)
    {
        AVIFileExit();
        return false;
    }

    AVIFILEINFO aviInfo;
    AVIFileInfo(_aviFile, &aviInfo, sizeof(AVIFILEINFO));
    _fps = (double) aviInfo.dwRate / aviInfo.dwScale;
    _size.cx = aviInfo.dwWidth;
    _size.cy = aviInfo.dwHeight;

    error = AVIFileGetStream(_aviFile, &_aviStream, streamtypeVIDEO, 0);
    if (error)
    {
        AVIFileRelease(_aviFile);
        AVIFileExit();
        return false;
    }*/

    _position.x = _position.y = 0;

    return true;
}

void CMoviePlayerControl::Destroy()
{
    Stop();
    if (_aviStream)
    {
        AVIStreamRelease(_aviStream);
        _aviStream = nullptr;
    }
    if (_aviFile != nullptr)
    {
        AVIFileRelease(_aviFile);
        _aviFile = nullptr;
    }
    AVIFileExit();
}

void DrawMovieFrame(CMoviePlayerControl *movieControl, unsigned char *data, CWindowControl *parent)
{
    BITMAPINFO bitmapInfo;
    BITMAPINFOHEADER bitmapInfoHeader;
    ZeroMemory(&bitmapInfo, sizeof(BITMAPINFO));
    RtlMoveMemory(&bitmapInfoHeader.biSize, data, sizeof(BITMAPINFOHEADER));
    bitmapInfo.bmiHeader = bitmapInfoHeader;

    if (bitmapInfoHeader.biSizeImage < 1)
    {
        return;
    }

    auto pixels = new char[bitmapInfoHeader.biSizeImage];
    RtlMoveMemory(pixels, data + sizeof(BITMAPINFOHEADER), bitmapInfoHeader.biSizeImage);

    BYTE memBitmapInfo[40];
    RtlMoveMemory(memBitmapInfo, &bitmapInfoHeader, sizeof(bitmapInfoHeader));

    void *bits;
    HDC dc = GetDC(parent->GetHWnd());
    HDC newDC = CreateCompatibleDC(dc);
    HBITMAP newBitmap = CreateDIBSection(dc, &bitmapInfo, DIB_RGB_COLORS, &bits, 0, 0);
    HBITMAP oldBitmap = (HBITMAP) SelectObject(newDC, newBitmap);
    SetDIBits(newDC, newBitmap, 0, bitmapInfoHeader.biHeight, pixels, &bitmapInfo, DIB_RGB_COLORS);

    RECT rect;
    SetRect(&rect, movieControl->GetX(), movieControl->GetY(),
            movieControl->GetX() + movieControl->GetWidth(), movieControl->GetY() + movieControl->GetHeight());
    BitBlt(dc, movieControl->GetX(), movieControl->GetY(), movieControl->GetWidth(), movieControl->GetHeight(), newDC,
           0, 0, SRCCOPY);
    movieControl->SetDrawingIndex(parent->SetDrawingImage(movieControl->GetDrawingIndex(), newDC, bitmapInfo, rect));
    delete[] pixels;

    SelectObject(newDC, oldBitmap);
    DeleteBitmap(newBitmap);
    DeleteDC(newDC);
    ReleaseDC(parent->GetHWnd(), dc);
}

void CMoviePlayerControl::Play()
{
    /*std::thread t([&](PAVISTREAM aviStream)
                  {
                      int firstFrame = AVIStreamStart(aviStream);
                      int numFrames = AVIStreamLength(aviStream);
                      PGETFRAME frame = AVIStreamGetFrameOpen(aviStream, NULL);

                      for (int i = firstFrame; i < numFrames && _playing; i++)
                      {
                          BYTE *data = (BYTE *) AVIStreamGetFrame(frame, i - firstFrame);
                          DrawMovieFrame(this, data, _parent);
                          CGameManager::GetInstance().Delay(60);
                      }
                      AVIStreamGetFrameClose(frame);

                      _playing = false;
                      CLuaTinker::GetLuaTinker().Call(_endEvent.c_str());
                  }, _aviStream);
    t.detach();*/
    _playing = true;

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

    Stop();
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

int CMoviePlayerControl::GetDrawingIndex()
{
    return _drawingIndex;
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
}