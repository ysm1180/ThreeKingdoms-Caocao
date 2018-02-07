#pragma once
#pragma comment(lib, "vfw32.lib")

#define MAX_AUDIO_FRAME_SIZE 192000

#include "BaseControl.h"

extern "C"  {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>

#include <SDL/include/SDL.h>
}

#include <string>
#include <Windows.h>
#include <Vfw.h>

namespace jojogame {
class CWindowControl;

struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
};

class CMoviePlayerControl
{
public:
    static void RegisterFunctions(lua_State *L);

    CMoviePlayerControl(CWindowControl *parent, std::wstring fileName);
    virtual ~CMoviePlayerControl();

    int GetX();
    int GetY();
    int GetWidth();
    int GetHeight();
    CWindowControl *GetParentControl();
    AVFormatContext *GetAVFormatContext();
    AVCodecContext *GetVideoCodecContext();
    AVCodecContext *GetAudioCodecContext();
    PacketQueue *GetPointerAudioQueue();
    int GetVideoStreamIndex();
    int GetAudioStreamIndex();
    double GetFps();
    bool IsPlaying();
    std::wstring GetEndEvent();
    int GetDrawingIndex();

    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetEndEvent(std::wstring endEvent);
    void SetFileName(std::wstring fileName);
    void SetDrawingIndex(int index);

    bool Create();
    void Destroy();

    void Play();
    void WaitForPlay();
    void Stop();

private:
    std::wstring _endEvent = L"";
    std::wstring _fileName = L"";
    bool _playing = false;
    POINT _position;
    SIZE _size;

    CWindowControl *_parent = nullptr;
    AVFormatContext *_formatContext = nullptr;
    AVCodecContext *_videoCodecContext = nullptr;
    AVCodecContext *_audioCodecContext = nullptr;
    int _videoStreamIndex = -1;
    int _audioStreamIndex = -1;
    double _fps = 0;
    PacketQueue _audioQueue;

    int _drawingIndex = 0;
};
}
