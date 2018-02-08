#pragma once
#pragma comment(lib, "vfw32.lib")

#define MAX_AUDIO_FRAME_SIZE 192000

#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)

#define FF_REFRESH_EVENT (SDL_USEREVENT)
#define FF_QUIT_EVENT (SDL_USEREVENT + 1)

#define VIDEO_FRAME_QUEUE_SIZE 1

#include "BaseControl.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>

#include <SDL/include/SDL.h>
#include <SDL/include/SDL_thread.h>

}

#include <string>
#include <Windows.h>
#include <Vfw.h>

namespace jojogame {
class CWindowControl;

struct PacketQueue
{
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
};

struct VideoFrame
{
    HDC dc;
    HBITMAP newBitmap, oldBitmap;
    uint8_t *buffer;
    int width, height;
    int allocated;
};

struct VideoState
{
    HWND parentControlHWnd;

    AVFormatContext *formatContext;
    AVCodecContext *videoCodecContext;
    AVCodecContext *audioCodecContext;
    int videoStreamIndex;
    int audioStreamIndex;
    AVStream *videoStream;
    AVStream *audioStream;
    uint8_t audioBuffer[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    unsigned int audioBufferSize;
    unsigned int audioBufferIndex;
    AVFrame audioFrame;
    AVPacket audioPacket;
    uint8_t *audioPacketData;
    int audioPacketSize;
    PacketQueue videoQueue;
    PacketQueue audioQueue;
    SwsContext *swsContext;

    VideoFrame frameQueue[VIDEO_FRAME_QUEUE_SIZE];
    int frameQueueSize, frameQueueRearIndex, frameQueueWIndex;
    SDL_mutex *frameQueueMutex;
    SDL_cond *frameQueueCond;

    SDL_Thread *parseThread;
    SDL_Thread *videoThread;

    SDL_mutex *screenMutex;

    std::string fileName;

    bool playing;
    POINT position;
};


class CMoviePlayerControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CMoviePlayerControl(CWindowControl *parent, std::string fileName);
    virtual ~CMoviePlayerControl();

    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();
    CWindowControl *GetParentControl();
    VideoState *GetVideoState();
    double GetFps();
    bool IsPlaying();
    std::wstring GetEndEvent();
    int GetDrawingIndex();

    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetEndEvent(std::wstring endEvent);
    void SetFileName(std::string fileName);
    void SetDrawingIndex(int index);

    bool Create();
    void Destroy();

    void Play();
    void WaitForPlay();
    void Stop();

private:
    VideoState _state;
    std::wstring _endEvent = L"";
    SIZE _size;

    CWindowControl *_parent = nullptr;
    double _fps = 0;

    int _drawingIndex = 0;
};
}
