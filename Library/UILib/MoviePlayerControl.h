#pragma once
#pragma comment(lib, "vfw32.lib")

#define MAX_AUDIO_FRAME_SIZE 192000

#define MAX_AUDIOQ_SIZE (5 * 16 * 1024)
#define MAX_VIDEOQ_SIZE (5 * 256 * 1024)

#define WM_REFRESH_EVENT (WM_USER)

#define AV_SYNC_THRESHOLD 0.01
#define AV_NOSYNC_THRESHOLD 10.0

#define SAMPLE_CORRECTION_PERCENT_MAX 10
#define AUDIO_DIFF_AVG_NB 20

#define VIDEO_FRAME_QUEUE_SIZE 1

#include "BaseControl.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>

#include <SDL/include/SDL.h>
#include <SDL/include/SDL_thread.h>

}

#include <Windows.h>

#include <string>
#include <sstream>
#include <mutex>
#include <chrono>
#include <thread>
#include <queue>

namespace jojogame {
class CWindowControl;

enum class SyncType
{
    AudioMaster = 0,
    VideoMaster = 1,
};

struct PacketQueue
{
    AVPacketList *firstPacket, *lastPacket;
    int size;
    std::mutex mutex;
    std::condition_variable cond;
};

struct VideoFrame
{
    HDC dc;
    HBITMAP newBitmap, oldBitmap;
    uint8_t *buffer;
    int width, height;
    double pts;
};

struct VideoState
{
    HWND parentControlHWnd;

    SyncType syncType;

    AVFormatContext *formatContext;

    AVCodecContext *videoCodecContext;
    int videoStreamIndex;
    AVStream *videoStream;
    PacketQueue videoQueue;

    AVCodecContext *audioCodecContext;
    int audioStreamIndex;
    AVStream *audioStream;
    PacketQueue audioQueue;
    uint8_t audioBuffer[(MAX_AUDIO_FRAME_SIZE * 3) / 2];
    unsigned int audioBufferSize;
    unsigned int audioBufferIndex;
    AVFrame audioFrame;
    AVPacket audioPacket;
    uint8_t *audioPacketData;
    int audioPacketSize;
    double audioDiffCum; /* used for AV difference average computation */
    double audioDiffAvgCoef;
    double audioDiffThreshold;
    int audioDiffAvgCount;

    double audioClock;
    int audioHwBufferSize;
    double frameTimer;
    double frameLastPts;
    double frameLastDelay;
    double videoClock; ///<pts of last decoded frame / predicted pts of next decoded frame
    double videoCurrentPts; ///<current displayed pts (different from video_clock if frame fifos are used)
    int64_t videoCurrentPtsTime;  ///<time (av_gettime) at which we updated video_current_pts - used to have running video pts

    SwsContext *swsContext;

    VideoFrame frameQueue[VIDEO_FRAME_QUEUE_SIZE];
    int frameQueueSize, frameQueueRearIndex, frameQueueWIndex;
    std::mutex frameQueueMutex;
    std::condition_variable frameQueueCond;

    std::mutex screenMutex;

    std::string fileName;

    bool playing;
    bool finishQueue;
    POINT position;

    std::queue<int> eventQueue;
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
    bool IsPlaying();

    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetEndEvent(std::wstring endEvent);

    bool Create();
    void Destroy();

    void Play();
    void Stop();

private:
    VideoState _state{};
    std::wstring _endEvent = L"";
    SIZE _size;

    CWindowControl *_parent = nullptr;
};
}
