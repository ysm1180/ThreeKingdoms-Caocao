#include "AudioPlayer.h"
#include "BaseLib/ConsoleOutput.h"
#include "BaseLib/MemoryStream.h"
#include "CommonLib/FileManager.h"
#include "CommonLib/ME5File.h"
#include "CommonLib/GameManager.h"
#include "BaseLib/MemoryPool.h"

namespace jojogame
{
void InitPacketAudioQueue(AudioPacketQueue *queue)
{
    queue->size = 0;
    queue->firstPacket = nullptr;
    queue->lastPacket = nullptr;
}

static int GetPacketAudioQueue(AudioState *audioState, AudioPacketQueue *queue, AVPacket *packet, int block)
{
    AVPacketList *packetList;
    int result;

    std::unique_lock<std::mutex> lock(queue->mutex);
    for (;;)
    {
        if (!audioState->playing)
        {
            result = -1;
            break;
        }

        packetList = queue->firstPacket;
        if (packetList)
        {
            queue->firstPacket = packetList->next;
            if (!queue->firstPacket)
            {
                queue->lastPacket = nullptr;
            }
            queue->size -= packetList->pkt.size;
            *packet = packetList->pkt;
            av_free(packetList);
            result = 1;
            break;
        }
        if (!block)
        {
            result = 0;
            break;
        }
        if (audioState->finishQueue)
        {
            audioState->playing = false;
            result = -1;
            break;
        }
        queue->cond.wait(lock);
    }
    lock.unlock();

    return result;
}

int PutPacketAudioQueue(AudioPacketQueue *queue, AVPacket *packet)
{
    AVPacketList *packetList = (AVPacketList *)av_malloc(sizeof(AVPacketList));
    if (!packetList)
    {
        return AVERROR(ENOMEM);
    }

    av_init_packet(&packetList->pkt);
    if (av_packet_ref(&packetList->pkt, packet) < 0)
    {
        return AVERROR(ENOMEM);
    }
    packetList->next = nullptr;

    std::unique_lock<std::mutex> lock(queue->mutex);
    if (!queue->lastPacket)
    {
        queue->firstPacket = packetList;
    }
    else
    {
        queue->lastPacket->next = packetList;
    }
    queue->lastPacket = packetList;
    queue->size += packetList->pkt.size;
    queue->cond.notify_one();
    lock.unlock();

    return 0;
}

static int ResamplingAudio(AVCodecContext *audio_decode_ctx,
                           AVFrame *audio_decode_frame,
                           enum AVSampleFormat out_sample_fmt,
                           int out_channels,
                           int out_sample_rate,
                           uint8_t *out_buf)
{
    SwrContext *swr_ctx = nullptr;
    int ret = 0;
    int64_t in_channel_layout = audio_decode_ctx->channel_layout;
    int64_t out_channel_layout = AV_CH_LAYOUT_STEREO;
    int out_nb_channels = 0;
    int out_linesize = 0;
    int in_nb_samples = 0;
    int out_nb_samples = 0;
    int max_out_nb_samples = 0;
    uint8_t **resampled_data = nullptr;
    int resampled_data_size = 0;

    swr_ctx = swr_alloc();
    if (!swr_ctx)
    {
        printf("swr_alloc error\n");
        return -1;
    }

    in_channel_layout = (audio_decode_ctx->channels ==
                         av_get_channel_layout_nb_channels(audio_decode_ctx->channel_layout))
                            ? audio_decode_ctx->channel_layout
                            : av_get_default_channel_layout(audio_decode_ctx->channels);
    if (in_channel_layout <= 0)
    {
        CConsoleOutput::OutputConsoles(L"in_channel_layout error\n");
        return -1;
    }

    if (out_channels == 1)
    {
        out_channel_layout = AV_CH_LAYOUT_MONO;
    }
    else if (out_channels == 2)
    {
        out_channel_layout = AV_CH_LAYOUT_STEREO;
    }
    else
    {
        out_channel_layout = AV_CH_LAYOUT_SURROUND;
    }

    in_nb_samples = audio_decode_frame->nb_samples;
    if (in_nb_samples <= 0)
    {
        CConsoleOutput::OutputConsoles(L"in_nb_samples error\n");
        return -1;
    }

    av_opt_set_int(swr_ctx, "in_channel_layout", in_channel_layout, 0);
    av_opt_set_int(swr_ctx, "in_sample_rate", audio_decode_ctx->sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "in_sample_fmt", audio_decode_ctx->sample_fmt, 0);

    av_opt_set_int(swr_ctx, "out_channel_layout", out_channel_layout, 0);
    av_opt_set_int(swr_ctx, "out_sample_rate", out_sample_rate, 0);
    av_opt_set_sample_fmt(swr_ctx, "out_sample_fmt", out_sample_fmt, 0);

    if ((ret = swr_init(swr_ctx)) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Failed to initialize the resampling context\n");
        return -1;
    }

    max_out_nb_samples = out_nb_samples = av_rescale_rnd(in_nb_samples,
                                                         out_sample_rate,
                                                         audio_decode_ctx->sample_rate,
                                                         AV_ROUND_UP);

    if (max_out_nb_samples <= 0)
    {
        CConsoleOutput::OutputConsoles(L"av_rescale_rnd error\n");
        return -1;
    }

    out_nb_channels = av_get_channel_layout_nb_channels(out_channel_layout);

    ret = av_samples_alloc_array_and_samples(&resampled_data, &out_linesize, out_nb_channels, out_nb_samples,
                                             out_sample_fmt, 0);
    if (ret < 0)
    {
        CConsoleOutput::OutputConsoles(L"av_samples_alloc_array_and_samples error\n");
        return -1;
    }

    out_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, audio_decode_ctx->sample_rate) + in_nb_samples,
                                    out_sample_rate, audio_decode_ctx->sample_rate, AV_ROUND_UP);
    if (out_nb_samples <= 0)
    {
        CConsoleOutput::OutputConsoles(L"av_rescale_rnd error\n");
        return -1;
    }

    if (out_nb_samples > max_out_nb_samples)
    {
        av_free(resampled_data[0]);
        ret = av_samples_alloc(resampled_data, &out_linesize, out_nb_channels, out_nb_samples, out_sample_fmt, 1);
        max_out_nb_samples = out_nb_samples;
    }

    if (swr_ctx)
    {
        ret = swr_convert(swr_ctx, resampled_data, out_nb_samples,
                          (const uint8_t **)audio_decode_frame->data, audio_decode_frame->nb_samples);
        if (ret < 0)
        {
            CConsoleOutput::OutputConsoles(L"swr_convert_error\n");
            return -1;
        }

        resampled_data_size = av_samples_get_buffer_size(&out_linesize, out_nb_channels, ret, out_sample_fmt, 1);
        if (resampled_data_size < 0)
        {
            CConsoleOutput::OutputConsoles(L"av_samples_get_buffer_size error\n");
            return -1;
        }
    }
    else
    {
        CConsoleOutput::OutputConsoles(L"swr_ctx null error\n");
        return -1;
    }

    memcpy(out_buf, resampled_data[0], resampled_data_size);

    if (resampled_data)
    {
        av_freep(&resampled_data[0]);
    }
    av_freep(&resampled_data);
    resampled_data = nullptr;

    if (swr_ctx)
    {
        swr_free(&swr_ctx);
    }
    return resampled_data_size;
}

int DecodeMusicFrame(AudioState *audioState, uint8_t *audioBuffer, int bufferSize)
{
    int dataSize = 0;
    AVPacket *packet = &audioState->audioPacket;

    for (;;)
    {
        while (audioState->audioPacketSize > 0)
        {
            int packetError = avcodec_send_packet(audioState->audioCodecContext, packet);
            if (packet->size < 0 || packetError < 0)
            {
                // If error, skip frame
                audioState->audioPacketSize = 0;
                break;
            }
            dataSize = 0;
            if (avcodec_receive_frame(audioState->audioCodecContext, &audioState->audioFrame) == 0)
            {
                dataSize = ResamplingAudio(audioState->audioCodecContext, &audioState->audioFrame, AV_SAMPLE_FMT_S16,
                                           audioState->audioFrame.channels, audioState->audioFrame.sample_rate,
                                           audioBuffer);
            }

            audioState->audioPacketData += packet->size;
            audioState->audioPacketSize -= packet->size;

            if (dataSize <= 0)
            {
                // No data yet, get more frames
                continue;
            }

            return dataSize;
        }

        if (packet->data)
        {
            av_packet_unref(packet);
        }

        if (GetPacketAudioQueue(audioState, &audioState->audioQueue, packet, 1) < 0)
        {
            return -1;
        }

        audioState->audioPacketData = packet->data;
        audioState->audioPacketSize = packet->size;

        if (packet->pts != AV_NOPTS_VALUE)
        {
            audioState->audioClock = packet->pts;
        }
    }
}

void MusicAudioCallback(void *userdata, Uint8 *stream, int len)
{
    auto audioState = (AudioState *)userdata;

    while (len > 0)
    {
        if (audioState->audioBufferIndex >= audioState->audioBufferSize)
        {
            // We have already sent all our data; get more
            int audioSize = DecodeMusicFrame(audioState, audioState->audioBuffer, sizeof(audioState->audioBuffer));
            if (audioSize < 0)
            {
                // If error, output silence
                audioState->audioBufferSize = 1024; // arbitrary?
                memset(audioState->audioBuffer, 0, audioState->audioBufferSize);
            }
            else
            {
                audioState->audioBufferSize = static_cast<unsigned int>(audioSize);
            }
            audioState->audioBufferIndex = 0;
        }

        int computedLen = audioState->audioBufferSize - audioState->audioBufferIndex;
        if (computedLen > len)
        {
            computedLen = len;
        }
        memcpy(stream, (uint8_t *)audioState->audioBuffer + audioState->audioBufferIndex, computedLen);
        len -= computedLen;
        stream += computedLen;
        audioState->audioBufferIndex += computedLen;
    }
}

void CAudioPlayerControl::RegisterFunctions(lua_State *L)
{
    LUA_BEGIN(CAudioPlayerControl, "_AudioPlayer");

    LUA_METHOD(IsPlaying);

    LUA_METHOD(Play);
    LUA_METHOD(Stop);
    LUA_METHOD(LoadFromFile);
    LUA_METHOD(LoadFromMe5File);
    LUA_METHOD(Destroy);
}

CAudioPlayerControl::CAudioPlayerControl()
{
}

CAudioPlayerControl::~CAudioPlayerControl()
{
    Destroy();
}

bool CAudioPlayerControl::IsPlaying()
{
    return _state.playing;
}

int CAudioPlayerControl::Read(void *opaque, unsigned char *buf, int buf_size)
{
    CMemoryStream *stream = static_cast<CMemoryStream *>(opaque);

    return stream->readsome(reinterpret_cast<char *>(buf), buf_size);
}

int64_t CAudioPlayerControl::Seek(void *opaque, int64_t offset, int whence)
{
    CMemoryStream *stream = static_cast<CMemoryStream *>(opaque);

    if (0x10000 == whence)
        return stream->Size();

    stream->seekg(offset, whence);
    return stream->tellg();
}

bool CAudioPlayerControl::LoadFromMe5File(std::wstring filePath, int groupIndex, int subIndex)
{
    AVCodecParameters *audioCodecParameters = nullptr;
    int error = 0;

    _state.formatContext = nullptr;

    CME5File me5File;
    me5File.Open(filePath);

    int size = me5File.GetItemByteSize(groupIndex, subIndex);
    auto *by = new BYTE[size];

    me5File.GetItemByteArr(by, groupIndex, subIndex);

    unsigned char *buffer = static_cast<unsigned char *>(av_malloc(size));
    _inputStream = new CMemoryStream(by, size);
    _state.formatContext = avformat_alloc_context();
    _state.formatContext->pb = avio_alloc_context(buffer, size, 0, _inputStream, &CAudioPlayerControl::Read, nullptr,
                                                  &CAudioPlayerControl::Seek);
    _state.formatContext->flags = AVFMT_FLAG_CUSTOM_IO;

    error = avformat_open_input(&_state.formatContext, "DUMMY", nullptr, nullptr);
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
        if (_state.formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO &&
            audioCodecParameters == nullptr)
        {
            audioCodecParameters = _state.formatContext->streams[i]->codecpar;
            _state.audioStream = _state.formatContext->streams[i];
            _state.audioStreamIndex = i;
        }
    }
    if (audioCodecParameters == nullptr)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find audio stream");
        return false;
    }

    AVCodec *audioCodec = avcodec_find_decoder(audioCodecParameters->codec_id);
    if (audioCodec == nullptr)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find audio decoder");
        return false;
    }

    _state.audioCodecContext = avcodec_alloc_context3(audioCodec);
    if (avcodec_parameters_to_context(_state.audioCodecContext, audioCodecParameters) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Failed to convert to context");
        return false;
    }

    if (avcodec_open2(_state.audioCodecContext, audioCodec, nullptr) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot open codec");
        return false;
    }

    delete[] by;

    return true;
}

bool CAudioPlayerControl::LoadFromFile(std::wstring filePath)
{
    filePath = CFileManager::GetInstance().GetFilePath(filePath);

    AVCodecParameters *audioCodecParameters = nullptr;
    int error = 0;

    _state.formatContext = nullptr;

    int length = WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, nullptr, 0, nullptr, nullptr);
    char *buffer = new char[length + 1];
    WideCharToMultiByte(CP_UTF8, 0, filePath.c_str(), -1, buffer, length, nullptr, nullptr);
    error = avformat_open_input(&_state.formatContext, buffer, nullptr, nullptr);
    delete[] buffer;
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
        if (_state.formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO &&
            audioCodecParameters == nullptr)
        {
            audioCodecParameters = _state.formatContext->streams[i]->codecpar;
            _state.audioStream = _state.formatContext->streams[i];
            _state.audioStreamIndex = i;
        }
    }
    if (audioCodecParameters == nullptr)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find audio stream");
        return false;
    }

    AVCodec *audioCodec = avcodec_find_decoder(audioCodecParameters->codec_id);
    if (audioCodec == nullptr)
    {
        CConsoleOutput::OutputConsoles(L"Cannot find audio decoder");
        return false;
    }

    _state.audioCodecContext = avcodec_alloc_context3(audioCodec);
    if (avcodec_parameters_to_context(_state.audioCodecContext, audioCodecParameters) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Failed to convert to context");
        return false;
    }

    if (avcodec_open2(_state.audioCodecContext, audioCodec, nullptr) < 0)
    {
        CConsoleOutput::OutputConsoles(L"Cannot open codec");
        return false;
    }

    return true;
}

void CAudioPlayerControl::Destroy()
{
    Stop();

    if (_audioThread != nullptr)
    {
        if (_audioThread->joinable())
        {
            _audioThread->join();
        }
        delete _audioThread;
        _audioThread = nullptr;
    }

    if (_inputStream != nullptr)
    {
        delete _inputStream;
        _inputStream = nullptr;
    }

    if (_state.audioCodecContext)
    {
        avcodec_free_context(&_state.audioCodecContext);
        _state.audioCodecContext = nullptr;
    }
    if (_state.formatContext)
    {
        avformat_close_input(&_state.formatContext);
        _state.formatContext = nullptr;
    }
}

bool PlaySound(AudioState *audioState)
{
    audioState->playing = true;
    audioState->finishQueue = false;
    audioState->maxPts = 0;
    audioState->audioBufferSize = 0;
    audioState->audioBufferIndex = 0;
    memset(&audioState->audioPacket, 0, sizeof(audioState->audioPacket));
    InitPacketAudioQueue(&audioState->audioQueue);

    audioState->audioPacketData = nullptr;
    audioState->audioPacketSize = 0;
    memset(&audioState->audioFrame, 0, sizeof(audioState->audioFrame));

    SDL_AudioSpec audioSpec{};
    auto audioContext = audioState->audioCodecContext;
    audioSpec.freq = audioContext->sample_rate;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = static_cast<Uint8>(audioContext->channels);
    audioSpec.silence = 0;
    audioSpec.samples = 8192;
    audioSpec.callback = MusicAudioCallback;
    audioSpec.userdata = audioState;

    if (SDL_OpenAudio(&audioSpec, nullptr))
    {
        return false;
    }

    SDL_PauseAudio(0);

    AVPacket packet{};
    av_init_packet(&packet);
    avcodec_flush_buffers(audioState->audioCodecContext);
    av_seek_frame(audioState->formatContext, audioState->audioStreamIndex, 0, 0);
    for (;;)
    {
        if (!audioState->playing)
        {
            audioState->audioQueue.cond.notify_all();
            break;
        }

        if (audioState->audioQueue.size > MAX_AUDIOQ_SIZE)
        {
            SDL_Delay(10);
            continue;
        }

        if (av_read_frame(audioState->formatContext, &packet) < 0)
        {
            if (audioState->formatContext->pb->error == 0)
            {
                audioState->finishQueue = true;
                audioState->audioQueue.cond.notify_all();
                continue;
            }
            audioState->playing = false;
            break;
        }

        if (packet.stream_index == audioState->audioStreamIndex)
        {
            PutPacketAudioQueue(&audioState->audioQueue, &packet);
            if (packet.pts != int64_t(AV_NOPTS_VALUE) && packet.pts > audioState->maxPts)
            {
                audioState->maxPts = packet.pts;
            }
        }
        else
        {
            av_packet_unref(&packet);
        }
    }

    return true;
}

void CAudioPlayerControl::Play(int playCount)
{
    _playCount = playCount;

    if (_state.playing && _audioThread)
    {
        if (_audioThread->joinable())
        {
            _audioThread->join();
        }
        return;
    }

    if (_stop)
    {
        if (_audioThread)
        {
            if (_audioThread->joinable())
            {
                _audioThread->join();
            }

            delete _audioThread;
            _audioThread = nullptr;
        }

        _stop = false;
        _audioThread = new std::thread([&]() {
            if (_playCount == 0)
            {
                while (!CGameManager::GetInstance().IsQuit() && !_stop)
                {
                    PlaySound(&_state);
                    SDL_Delay(250);
                    SDL_CloseAudio();
                }
            }
            else
            {
                for (int count = 0; count < _playCount; ++count)
                {
                    if (!_stop)
                    {
                        PlaySound(&_state);
                        SDL_Delay(250);
                        SDL_CloseAudio();
                    }
                }
            }

            _state.playing = false;
            _stop = true;
        });
    }
}

void CAudioPlayerControl::Stop()
{
    if (_state.playing)
    {
        _stop = true;

        _state.playing = false;
        if (_audioThread)
        {
            if (_audioThread->joinable())
            {
                _audioThread->join();
            }
            delete _audioThread;
            _audioThread = nullptr;
        }
    }
}
} // namespace jojogame
