#include "MoviePlayerControl.h"

#include <iostream>

#include "BaseLib/ConsoleOutput.h"
#include "CommonLib/FileManager.h"
#include "ControlManager.h"
#include "ToolbarControl.h"
#include "WindowControl.h"

namespace three_kingdoms {
void CMoviePlayerControl::RegisterFunctions(lua_State *L) {
  LUA_BEGIN(CMoviePlayerControl, "_MoviePlayer");

  LUA_METHOD(GetX);
  LUA_METHOD(GetY);
  LUA_METHOD(GetLuaY);
  LUA_METHOD(GetWidth);
  LUA_METHOD(GetHeight);
  LUA_METHOD(IsPlaying);
  LUA_METHOD(SetX);
  LUA_METHOD(SetY);
  LUA_METHOD(SetLuaY);
  LUA_METHOD(SetWidth);
  LUA_METHOD(SetHeight);
  LUA_METHOD(SetEndEvent);

  LUA_METHOD(Play);
  LUA_METHOD(Stop);
  LUA_METHOD(Create);
  LUA_METHOD(Destroy);
}

CMoviePlayerControl::CMoviePlayerControl(CWindowControl *parent,
                                         std::wstring fileName) {
  if (parent) {
    _parent = parent;
    _state.parentControlHWnd = parent->GetHWnd();
  }
  _state.fileName = CFileManager::GetInstance().GetFilePath(fileName);
}

CMoviePlayerControl::~CMoviePlayerControl() {
  Destroy();
  if (_endEvent != LUA_NOREF) {
    luaL_unref(CLuaTinker::GetLuaTinker().GetLuaState(), LUA_REGISTRYINDEX,
               _endEvent);
  }
}

bool CMoviePlayerControl::IsPlaying() { return _state.playing; }

void CMoviePlayerControl::SetEndEvent() {
  auto l = CLuaTinker::GetLuaTinker().GetLuaState();
  if (lua_isfunction(l, -1)) {
    lua_pushvalue(l, -1);
    _endEvent = luaL_ref(l, LUA_REGISTRYINDEX);
  }

  lua_pop(l, 1);
}

void InitPacketQueue(PacketQueue *queue) {
  queue->size = 0;
  queue->list.clear();
}

int PutPacketQueue(PacketQueue *queue, AVPacket *packet) {
  AVPacket clonePacket{0};
  if (av_packet_ref(&clonePacket, packet) < 0) {
    return AVERROR(ENOMEM);
  }

  std::unique_lock<std::mutex> lock(queue->mutex);
  queue->list.push_back(clonePacket);
  queue->size += clonePacket.size;
  queue->cond.notify_one();
  lock.unlock();

  return 0;
}

static int GetPacketQueue(VideoState *videoState, PacketQueue *queue,
                          AVPacket *packet, int block) {
  int result;

  std::unique_lock<std::mutex> lock(queue->mutex);
  for (;;) {
    if (!videoState->playing) {
      result = -1;
      break;
    }

    if (!queue->list.empty()) {
      AVPacket clonePacket = queue->list.front();
      queue->list.pop_front();
      queue->size -= clonePacket.size;
      *packet = clonePacket;
      result = 1;
      break;
    }
    if (!block) {
      result = 0;
      break;
    }
    if (videoState->finishQueue) {
      videoState->playing = false;
      result = -1;
      break;
    }
    queue->cond.wait(lock);
  }
  lock.unlock();

  return result;
}

double GetAudioClock(VideoState *videoState) {
  double pts = videoState->audioClock; /* maintained in the audio thread */
  int hwBufSize = videoState->audioBufferSize - videoState->audioBufferIndex;
  int bytesPerSec = 0;
  int n = videoState->audioCodecContext->channels * 2;

  if (videoState->audioStream) {
    bytesPerSec = videoState->audioCodecContext->sample_rate * n;
  }
  if (bytesPerSec) {
    pts -= (double)hwBufSize / bytesPerSec;
  }
  return pts;
}

double GetVideoClock(VideoState *is) {
  double delta = (av_gettime() - is->videoCurrentPtsTime) / 1000000.0;
  return is->videoCurrentPts + delta;
}

double GetMasterClock(VideoState *videoState) {
  if (videoState->syncType == SyncType::AudioMaster) {
    return GetAudioClock(videoState);
  }
  return GetVideoClock(videoState);
}

int SynchronizeAudio(VideoState *videoState, short *samples, int samplesSize) {
  int n = 2 * videoState->audioCodecContext->channels;

  if (videoState->syncType != SyncType::AudioMaster) {
    double refClock = GetMasterClock(videoState);
    double diff = GetAudioClock(videoState) - refClock;

    if (diff < AV_NOSYNC_THRESHOLD) {
      // accumulate the diffs
      videoState->audioDiffCum =
          diff + videoState->audioDiffAvgCoef * videoState->audioDiffCum;
      if (videoState->audioDiffAvgCount < AUDIO_DIFF_AVG_NB) {
        videoState->audioDiffAvgCount++;
      } else {
        double avgDiff =
            videoState->audioDiffCum * (1.0 - videoState->audioDiffAvgCoef);
        if (fabs(avgDiff) >= videoState->audioDiffThreshold) {
          int wantedSize =
              samplesSize +
              ((int)(diff * videoState->audioCodecContext->sample_rate) * n);
          int minSize =
              samplesSize * ((100 - SAMPLE_CORRECTION_PERCENT_MAX) / 100);
          int maxSize =
              samplesSize * ((100 + SAMPLE_CORRECTION_PERCENT_MAX) / 100);

          if (wantedSize < minSize) {
            wantedSize = minSize;
          } else if (wantedSize > maxSize) {
            wantedSize = maxSize;
          }
          if (wantedSize < samplesSize) {
            /* remove samples */
            samplesSize = wantedSize;
          } else if (wantedSize > samplesSize) {
            /* add samples by copying final sample*/
            int nb = (samplesSize - wantedSize);
            uint8_t *samples_end = (uint8_t *)samples + samplesSize - n;
            uint8_t *q = samples_end + n;

            while (nb > 0) {
              memcpy(q, samples_end, n);
              q += n;
              nb -= n;
            }
            samplesSize = wantedSize;
          }
        }
      }
    } else {
      /* difference videoState TOO big; reset diff stuff */
      videoState->audioDiffAvgCount = 0;
      videoState->audioDiffCum = 0;
    }
  }
  return samplesSize;
}

int DecodeAudioFrame(VideoState *videoState, uint8_t *audioBuffer,
                     int bufferSize, double *ptsPtr) {
  int dataSize = 0;
  AVPacket *packet = &videoState->audioPacket;

  for (;;) {
    while (videoState->audioPacketSize > 0) {
      int packetError =
          avcodec_send_packet(videoState->audioCodecContext, packet);
      if (packet->size < 0 || packetError < 0) {
        // If error, skip frame
        videoState->audioPacketSize = 0;
        break;
      }
      dataSize = 0;
      if (avcodec_receive_frame(videoState->audioCodecContext,
                                &videoState->audioFrame) == 0) {
        dataSize = av_samples_get_buffer_size(
            nullptr, videoState->audioCodecContext->channels,
            videoState->audioFrame.nb_samples,
            videoState->audioCodecContext->sample_fmt, 1);
        memcpy(audioBuffer, videoState->audioFrame.data[0],
               static_cast<size_t>(dataSize));
      }

      videoState->audioPacketData += packet->size;
      videoState->audioPacketSize -= packet->size;

      if (dataSize <= 0) {
        // No data yet, get more frames
        continue;
      }

      double pts = videoState->audioClock;
      *ptsPtr = pts;
      int n = 2 * videoState->audioCodecContext->channels;
      videoState->audioClock +=
          (double)dataSize /
          (double)(n * videoState->audioCodecContext->sample_rate);
      // We have data, return it and come back for more later
      return dataSize;
    }

    if (packet->data) {
      av_packet_unref(packet);
    }

    if (!videoState->playing ||
        GetPacketQueue(videoState, &videoState->audioQueue, packet, 1) < 0) {
      return -1;
    }

    videoState->audioPacketData = packet->data;
    videoState->audioPacketSize = packet->size;

    if (packet->pts != AV_NOPTS_VALUE) {
      videoState->audioClock =
          av_q2d(videoState->audioStream->time_base) * packet->pts;
    }
  }
}

void AudioCallback(void *userdata, Uint8 *stream, int len) {
  auto videoState = (VideoState *)userdata;
  double pts;

  while (videoState->playing && len > 0) {
    if (videoState->audioBufferIndex >= videoState->audioBufferSize) {
      // We have already sent all our data; get more
      int audioSize = DecodeAudioFrame(videoState, videoState->audioBuffer,
                                       sizeof(videoState->audioBuffer), &pts);
      if (audioSize < 0) {
        // If error, output silence
        videoState->audioBufferSize = 1024;  // arbitrary?
        memset(videoState->audioBuffer, 0, videoState->audioBufferSize);
      } else {
        audioSize = SynchronizeAudio(
            videoState, (int16_t *)videoState->audioBuffer, audioSize);
        videoState->audioBufferSize = static_cast<unsigned int>(audioSize);
      }
      videoState->audioBufferIndex = 0;
    }

    int computedLen =
        videoState->audioBufferSize - videoState->audioBufferIndex;
    if (computedLen > len) {
      computedLen = len;
    }
    memcpy(stream,
           (uint8_t *)videoState->audioBuffer + videoState->audioBufferIndex,
           computedLen);
    len -= computedLen;
    stream += computedLen;
    videoState->audioBufferIndex += computedLen;
  }
}

static void RefreshTimerCallback(void *data) {
  auto videoState = (VideoState *)data;
  videoState->eventQueue.push(WM_REFRESH_EVENT);

  //    PostThreadMessage(threadId, WM_REFRESH_EVENT, NULL, (LPARAM)data);

  //    SDL_Event event;
  //    event.type = WM_REFRESH_EVENT;
  //    event.user.data1 = data;
  //    SDL_PushEvent(&event);
  //    return 0; /* 0 means stop timer */
}

/* schedule a video refresh in 'delay' ms */
static void ScheduleRefresh(VideoState *is, int delay) {
  //    auto mainThreadId = std::this_thread::get_id();
  //    std::stringstream stringStream;
  //    stringStream << mainThreadId;
  //    auto id = (DWORD) std::stoull(stringStream.str());

  std::thread([is, delay]() {
    std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    RefreshTimerCallback(is);
  }).detach();

  //    SDL_AddTimer(delay, RefreshTimerCallback, is);
}

void DisplayVideo(VideoState *videoState) {
  VideoFrame *videoFrame =
      &videoState->frameQueue[videoState->frameQueueRearIndex];
  if (videoFrame->dc) {
    std::unique_lock<std::mutex> lock(videoState->screenMutex);

    if (videoState->playing) {
      HDC dc = GetDC(videoState->parentControlHWnd);
      BitBlt(dc, videoState->position.x, videoState->position.y,
             videoState->videoCodecContext->width,
             videoState->videoCodecContext->height, videoFrame->dc, 0, 0,
             SRCCOPY);
      ReleaseDC(videoState->parentControlHWnd, dc);
    }

    lock.unlock();
  }
}

void RefreshVideoTimer(void *userdata) {
  auto *videoState = (VideoState *)userdata;

  if (videoState->videoStream) {
    if (videoState->frameQueueSize == 0) {
      ScheduleRefresh(videoState, 1);
    } else {
      VideoFrame *videoFrame =
          &videoState->frameQueue[videoState->frameQueueRearIndex];

      videoState->videoCurrentPts = videoFrame->pts;
      videoState->videoCurrentPtsTime = av_gettime();
      double delay =
          videoFrame->pts - videoState->frameLastPts;  // the pts from last time
      if (delay <= 0 || delay >= 1.0) {
        // if incorrect delay, use previous one
        delay = videoState->frameLastDelay;
      }
      // save for next time
      videoState->frameLastDelay = delay;
      videoState->frameLastPts = videoFrame->pts;

      // update delay to sync to audio if not master source
      if (videoState->syncType != SyncType::VideoMaster) {
        double refClock = GetMasterClock(videoState);
        double diff = videoFrame->pts - refClock;

        // Skip or repeat the frame. Take delay into account
        // FFPlay still doesn't "know if this is the best guess."
        double syncThreshold =
            (delay > AV_SYNC_THRESHOLD) ? delay : AV_SYNC_THRESHOLD;
        if (fabs(diff) < AV_NOSYNC_THRESHOLD) {
          if (diff <= -syncThreshold) {
            delay = 0;
          } else if (diff >= syncThreshold) {
            delay = 2 * delay;
          }
        }
      }
      videoState->frameTimer += delay;
      // computer the REAL delay
      double actualDelay = videoState->frameTimer - (av_gettime() / 1000000.0);
      if (actualDelay < 0.010) {
        // Really it should skip the picture instead
        actualDelay = 0.010;
      }
      ScheduleRefresh(videoState, (int)(actualDelay * 1000 + 0.5));

      DisplayVideo(videoState);

      if (++videoState->frameQueueRearIndex == VIDEO_FRAME_QUEUE_SIZE) {
        videoState->frameQueueRearIndex = 0;
      }

      std::unique_lock<std::mutex> lock(videoState->frameQueueMutex);
      videoState->frameQueueSize--;
      videoState->frameQueueCond.notify_one();
      lock.unlock();
    }
  } else {
    ScheduleRefresh(videoState, 100);
  }
}

void AllocPicture(void *userdata) {
  auto *videoState = (VideoState *)userdata;
  VideoFrame *videoFrame =
      &videoState->frameQueue[videoState->frameQueueWIndex];

  if (videoFrame->dc) {
    SelectObject(videoFrame->dc, videoFrame->oldBitmap);
    DeleteBitmap(videoFrame->newBitmap);
    DeleteDC(videoFrame->dc);
  }

  std::unique_lock<std::mutex> lock(videoState->screenMutex);

  int numBytes = av_image_get_buffer_size(
      AV_PIX_FMT_RGB24, videoState->videoCodecContext->width,
      videoState->videoCodecContext->height, 32);
  if (numBytes < 0) {
    CConsoleOutput::OutputConsoles(L"Cannot get image buffer size");
    return;
  }
  videoFrame->buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biBitCount = 24;
  bmi.bmiHeader.biCompression = BI_RGB;
  bmi.bmiHeader.biHeight = -videoState->videoCodecContext->height;
  bmi.bmiHeader.biWidth = videoState->videoCodecContext->width;
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biSizeImage =
      static_cast<DWORD>(videoState->videoCodecContext->height *
                         videoState->videoCodecContext->width * 3);

  HDC dc = GetDC(videoState->parentControlHWnd);
  videoFrame->dc = CreateCompatibleDC(dc);
  videoFrame->newBitmap =
      CreateDIBSection(videoFrame->dc, &bmi, DIB_RGB_COLORS,
                       (void **)&videoFrame->buffer, nullptr, 0);
  videoFrame->oldBitmap =
      (HBITMAP)SelectObject(videoFrame->dc, videoFrame->newBitmap);

  ReleaseDC(videoState->parentControlHWnd, dc);

  lock.unlock();

  videoFrame->width = videoState->videoCodecContext->width;
  videoFrame->height = videoState->videoCodecContext->height;
}

int QueuePicture(VideoState *videoState, AVFrame *frame, double pts) {
  // wait until we have space for a new pic
  std::unique_lock<std::mutex> lock(videoState->frameQueueMutex);
  while (videoState->frameQueueSize >= VIDEO_FRAME_QUEUE_SIZE &&
         videoState->playing) {
    videoState->frameQueueCond.wait(lock);
  }
  lock.unlock();

  if (!videoState->playing) {
    return -1;
  }

  VideoFrame *videoFrame =
      &videoState->frameQueue[videoState->frameQueueWIndex];

  if (!videoFrame->buffer ||
      videoFrame->width != videoState->videoCodecContext->width ||
      videoFrame->height != videoState->videoCodecContext->height) {
    AllocPicture(videoState);
    if (!videoState->playing) {
      return -1;
    }
  }

  if (videoFrame->buffer) {
    videoFrame->pts = pts;
    AVFrame *frameBGR = av_frame_alloc();

    av_image_fill_arrays(frameBGR->data, frameBGR->linesize, videoFrame->buffer,
                         AV_PIX_FMT_RGB24, videoState->videoCodecContext->width,
                         videoState->videoCodecContext->height, 32);

    sws_scale(videoState->swsContext, (uint8_t const *const *)frame->data,
              frame->linesize, 0, videoState->videoCodecContext->height,
              frameBGR->data, frameBGR->linesize);

    frameBGR->data[0] = videoFrame->buffer;
    frameBGR->linesize[0] = videoState->videoCodecContext->width * 3;

    if (++videoState->frameQueueWIndex == VIDEO_FRAME_QUEUE_SIZE) {
      videoState->frameQueueWIndex = 0;
    }

    std::unique_lock<std::mutex> lock(videoState->frameQueueMutex);
    videoState->frameQueueSize++;
    lock.unlock();
    av_frame_free(&frameBGR);
  }

  return 0;
}

double SynchronizeVideo(VideoState *videoState, AVFrame *srcFrame, double pts) {
  if (pts != 0) {
    // if we have pts, set video clock to it
    videoState->videoClock = pts;
  } else {
    // if we aren't given a pts, set it to the clock
    pts = videoState->videoClock;
  }

  // update the video clock
  double frameDelay = av_q2d(videoState->videoCodecContext->time_base);
  // if we are repeating a frame, adjust clock accordingly
  frameDelay += srcFrame->repeat_pict * (frameDelay * 0.5);
  videoState->videoClock += frameDelay;
  return pts;
}

int ThreadVideo(void *arg) {
  VideoState *videoState = (VideoState *)arg;
  AVPacket packet;

  AVFrame *frame = av_frame_alloc();

  for (; videoState->playing;) {
    if (!videoState->playing ||
        GetPacketQueue(videoState, &videoState->videoQueue, &packet, 1) < 0) {
      break;
    }

    double pts = 0;
    if (avcodec_send_packet(videoState->videoCodecContext, &packet) == 0) {
      if (avcodec_receive_frame(videoState->videoCodecContext, frame) == 0) {
        if ((pts = frame->best_effort_timestamp) == AV_NOPTS_VALUE) {
          pts = 0;
        }
        pts *= av_q2d(videoState->videoStream->time_base);
        pts = SynchronizeVideo(videoState, frame, pts);
        if (QueuePicture(videoState, frame, pts) < 0) {
          break;
        }
      }
    }
    av_packet_unref(&packet);
  }

  av_frame_free(&frame);
  return 0;
}

bool CMoviePlayerControl::Create() {
  AVCodecParameters *videoCodecParameters = nullptr;
  AVCodecParameters *audioCodecParameters = nullptr;
  int error = 0;

  _state.formatContext = nullptr;

  int length = WideCharToMultiByte(CP_UTF8, 0, _state.fileName.c_str(), -1,
                                   nullptr, 0, nullptr, nullptr);
  char *buffer = new char[length + 1];
  WideCharToMultiByte(CP_UTF8, 0, _state.fileName.c_str(), -1, buffer, length,
                      nullptr, nullptr);
  error = avformat_open_input(&_state.formatContext, buffer, nullptr, nullptr);
  delete[] buffer;
  if (error < 0) {
    CConsoleOutput::OutputConsoles(L"File open error");
    return false;
  }

  if (avformat_find_stream_info(_state.formatContext, nullptr) < 0) {
    CConsoleOutput::OutputConsoles(L"Cannot find stream information");
    return false;
  }

  // Find video stream
  for (unsigned int i = 0; i < _state.formatContext->nb_streams; i++) {
    if (_state.formatContext->streams[i]->codecpar->codec_type ==
            AVMEDIA_TYPE_VIDEO &&
        videoCodecParameters == nullptr) {
      videoCodecParameters = _state.formatContext->streams[i]->codecpar;
      _state.videoStream = _state.formatContext->streams[i];
      _state.videoStreamIndex = i;
    }
    if (_state.formatContext->streams[i]->codecpar->codec_type ==
            AVMEDIA_TYPE_AUDIO &&
        audioCodecParameters == nullptr) {
      audioCodecParameters = _state.formatContext->streams[i]->codecpar;
      _state.audioStream = _state.formatContext->streams[i];
      _state.audioStreamIndex = i;
    }
  }
  if (videoCodecParameters == nullptr || audioCodecParameters == nullptr) {
    CConsoleOutput::OutputConsoles(L"Cannot find video or audio stream");
    return false;
  }

  const AVCodec *videoCodec =
      avcodec_find_decoder(videoCodecParameters->codec_id);
  if (videoCodec == nullptr) {
    CConsoleOutput::OutputConsoles(L"Cannot find video decoder");
    return false;
  }
  const AVCodec *audioCodec =
      avcodec_find_decoder(audioCodecParameters->codec_id);
  if (audioCodec == nullptr) {
    CConsoleOutput::OutputConsoles(L"Cannot find audio decoder");
    return false;
  }

  _state.videoCodecContext = avcodec_alloc_context3(videoCodec);
  if (avcodec_parameters_to_context(_state.videoCodecContext,
                                    videoCodecParameters) < 0) {
    CConsoleOutput::OutputConsoles(L"Failed to convert to context");
    return false;
  }
  _state.audioCodecContext = avcodec_alloc_context3(audioCodec);
  if (avcodec_parameters_to_context(_state.audioCodecContext,
                                    audioCodecParameters) < 0) {
    CConsoleOutput::OutputConsoles(L"Failed to convert to context");
    return false;
  }

  if (avcodec_open2(_state.videoCodecContext, videoCodec, nullptr) < 0) {
    CConsoleOutput::OutputConsoles(L"Cannot open codec");
    return false;
  }
  if (avcodec_open2(_state.audioCodecContext, audioCodec, nullptr) < 0) {
    CConsoleOutput::OutputConsoles(L"Cannot open codec");
    return false;
  }

  _state.syncType = SyncType::VideoMaster;

  _state.audioBufferSize = 0;
  _state.audioBufferIndex = 0;
  memset(&_state.audioPacket, 0, sizeof(_state.audioPacket));
  InitPacketQueue(&_state.videoQueue);
  InitPacketQueue(&_state.audioQueue);
  av_q2d(
      _state.formatContext->streams[_state.videoStreamIndex]->avg_frame_rate);

  int width = _state.videoCodecContext->width;
  int height = _state.videoCodecContext->height;
  _state.swsContext = nullptr;
  _state.swsContext = sws_getCachedContext(
      _state.swsContext, width, height, _state.videoCodecContext->pix_fmt,
      width, height, AV_PIX_FMT_BGR24, SWS_BICUBIC, nullptr, nullptr, nullptr);

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

void CMoviePlayerControl::Destroy() {
  Stop();

  if (_state.audioCodecContext) {
    avcodec_free_context(&_state.audioCodecContext);
    _state.audioCodecContext = nullptr;
  }
  if (_state.videoCodecContext) {
    avcodec_free_context(&_state.videoCodecContext);
    _state.videoCodecContext = nullptr;
  }
  if (_state.formatContext) {
    avformat_close_input(&_state.formatContext);
    _state.formatContext = nullptr;
  }
}

bool PlaySound(VideoState *videoState) {
  SDL_AudioSpec audioSpec{};
  auto audioContext = videoState->audioCodecContext;
  audioSpec.freq = audioContext->sample_rate;
  audioSpec.format = AUDIO_S16SYS;
  audioSpec.channels = static_cast<Uint8>(audioContext->channels);
  audioSpec.silence = 0;
  audioSpec.samples = 8192;
  audioSpec.callback = AudioCallback;
  audioSpec.userdata = videoState;

  if (SDL_OpenAudio(&audioSpec, nullptr)) {
    return false;
  }

  videoState->audioHwBufferSize = audioSpec.size;
  SDL_PauseAudio(0);
  return true;
}

bool PlayMovie(VideoState *videoState) {
  AVPacket packet;

  videoState->frameTimer = (double)av_gettime() / 1000000.0;
  videoState->frameLastDelay = 40e-3;
  videoState->videoCurrentPtsTime = av_gettime();

  auto t = std::thread([&]() { ThreadVideo(videoState); });

  for (;;) {
    if (!videoState->playing) {
      videoState->frameQueueCond.notify_all();
      break;
    }

    // seek stuff goes here
    if (videoState->audioQueue.size > MAX_AUDIOQ_SIZE ||
        videoState->videoQueue.size > MAX_VIDEOQ_SIZE) {
      SDL_Delay(10);
      continue;
    }

    if (av_read_frame(videoState->formatContext, &packet) < 0) {
      if (videoState->formatContext->pb->error == 0) {
        videoState->finishQueue = true;
        videoState->videoQueue.cond.notify_all();
        videoState->audioQueue.cond.notify_all();
        continue;
      }
      videoState->playing = false;
      break;
    }

    if (packet.stream_index == videoState->videoStreamIndex) {
      PutPacketQueue(&videoState->videoQueue, &packet);
    } else if (packet.stream_index == videoState->audioStreamIndex) {
      PutPacketQueue(&videoState->audioQueue, &packet);
    } else {
      av_packet_unref(&packet);
    }
  }

  t.join();

  return true;
}

void CMoviePlayerControl::Play() {
  std::queue<int> queue;
  _state.eventQueue.swap(queue);
  _state.playing = true;
  _state.finishQueue = false;

  ScheduleRefresh(&_state, 40);

  auto t = std::thread([&]() {
    PlaySound(&_state);
    PlayMovie(&_state);
  });

  //    SDL_Event event;
  //    for (; _state.playing;)
  //    {
  //        SDL_WaitEvent(&event);
  //        OutputDebugString(L"Event\n");
  //        switch (event.type)
  //        {
  //            case SDL_QUIT:
  //                SDL_Quit();
  //                break;
  //            case WM_REFRESH_EVENT:
  //                RefreshVideoTimer(event.user.data1);
  //                break;
  //            default:
  //                break;
  //        }
  //    }

  auto quit = false;
  MSG message;

  for (; _state.playing;) {
    if (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE)) {
      if (message.message == WM_QUIT) {
        Destroy();
        quit = true;
        break;
      }

      TranslateMessage(&message);
      DispatchMessage(&message);
    } else if (!_state.playing) {
      break;
    } else if (!_state.eventQueue.empty() &&
               _state.eventQueue.front() == WM_REFRESH_EVENT) {
      _state.eventQueue.pop();
      RefreshVideoTimer(reinterpret_cast<void *>(&_state));
    }
  }

  t.join();

  if (_endEvent != LUA_NOREF) {
    CLuaTinker::GetLuaTinker().Call(_endEvent, this);
  }

  _parent->Refresh();
  SDL_CloseAudio();

  if (quit) {
    PostQuitMessage(0);
  }
}

void CMoviePlayerControl::Stop() {
  if (_state.playing) {
    _state.playing = false;
    SDL_CloseAudio();
  }
}

int CMoviePlayerControl::GetX() { return _state.position.x; }

int CMoviePlayerControl::GetLuaY() {
  int toolbarHeight = 0;
  auto toolbar = _parent->GetToolbar();
  if (toolbar) {
    toolbarHeight = toolbar->GetHeight();
  }
  return this->GetY() - toolbarHeight;
}

int CMoviePlayerControl::GetY() { return _state.position.y; }

int CMoviePlayerControl::GetWidth() { return _size.cx; }

int CMoviePlayerControl::GetHeight() { return _size.cy; }

void CMoviePlayerControl::SetX(int x) { _state.position.x = x; }

void CMoviePlayerControl::SetLuaY(int y) {
  auto toolbar = _parent->GetToolbar();
  if (toolbar) {
    y += toolbar->GetHeight();
  }
  this->SetY(y);
}

void CMoviePlayerControl::SetY(int y) { _state.position.y = y; }

void CMoviePlayerControl::SetWidth(int width) { _size.cx = width; }

void CMoviePlayerControl::SetHeight(int height) { _size.cy = height; }
}  // namespace three_kingdoms