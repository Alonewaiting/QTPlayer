#pragma once
#ifdef __cplusplus
extern "C"
{
#endif
#include "libavformat/avformat.h"
#ifdef __cplusplus
}
#endif
#include <string>
struct AVFormatContext;
struct AVCodecContext;
struct AVCodec;
struct AVFrame;
struct AVPacket;



class FFFFmpegDecode {
public:
    FFFFmpegDecode(const std::string & filePath = "");
public:
    void init();
    AVFrame* getNextFrame();
    void releaseFrame(AVFrame* frame);
    
private: 
    std::string m_filePath;
    int m_videoIndex = 0;
    AVFormatContext* m_pFormatCtx = nullptr;
    AVBufferRef* m_hw_device_ctx = nullptr;
    enum AVPixelFormat m_hw_pix_fmt = AV_PIX_FMT_NONE;
    AVCodecContext* m_pCodercCtxVideo = nullptr;
    AVCodecContext* m_pCodercCtxAudio = nullptr;
    AVCodec* m_pCodercVideo = nullptr;
    AVCodec* m_pCodercAudio = nullptr;
    AVFrame* m_pFrame = nullptr;
    AVFrame* m_tempFrame = nullptr;
    AVFrame* m_pFrameYUV = nullptr;
    AVPacket* m_pPacket = nullptr;

};