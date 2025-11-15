#include "FFFmpegDecode.h"
#include "LogUtils.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include"libswresample/swresample.h"
#include "libavutil/avutil.h"
#include "libavutil/imgutils.h"
#ifdef __cplusplus
}
#endif


static enum AVPixelFormat get_hw_format(AVCodecContext* ctx,
    const enum AVPixelFormat* pix_fmts)
{
    const enum AVPixelFormat* p;

    for (p = pix_fmts; *p != -1; p++) {
        if (*p == AV_PIX_FMT_D3D11)
            return *p;
    }

    fprintf(stderr, "Failed to get HW surface format.\n");
    return AV_PIX_FMT_NONE;
}

FFFFmpegDecode::FFFFmpegDecode(const std::string& filePath) :m_filePath(filePath)
{ 
    init();
}

void FFFFmpegDecode::init()
{
    if (m_filePath.empty()) {
        return;
    }
    auto type = av_hwdevice_find_type_by_name("d3d11va");
    m_pFormatCtx = avformat_alloc_context();
    //open input
    if (0 != avformat_open_input(&this->m_pFormatCtx, this->m_filePath.c_str(), nullptr, nullptr)) {
        VPLOGE("av format open failed");
        return ;
    }
    if (0 > avformat_find_stream_info(this->m_pFormatCtx, nullptr)) {
        VPLOGE("avformat find stream info erro");
        return;
    }
    this->m_videoIndex = av_find_best_stream(this->m_pFormatCtx, AVMEDIA_TYPE_VIDEO, -1, -1, (const AVCodec**)&m_pCodercVideo, 0);
    for (int i = 0;; i++) {
        const AVCodecHWConfig* config = avcodec_get_hw_config(m_pCodercVideo, i);
        if (!config) {
            return;
        }
        if (config->methods & AV_CODEC_HW_CONFIG_METHOD_HW_DEVICE_CTX &&
            config->device_type == type) {
            m_hw_pix_fmt = config->pix_fmt;
            break;
        }
    }
    m_pCodercCtxVideo = avcodec_alloc_context3(m_pCodercVideo);
    avcodec_parameters_to_context(this->m_pCodercCtxVideo, this->m_pFormatCtx->streams[this->m_videoIndex]->codecpar);
    if (m_hw_pix_fmt == AV_PIX_FMT_D3D11) {
        auto ret = av_hwdevice_ctx_create(&m_hw_device_ctx, type, NULL, NULL, 0);
        if (ret != 0) {
            return;
        }
        m_pCodercCtxVideo->hw_device_ctx = av_buffer_ref(m_hw_device_ctx);
        m_pCodercCtxVideo->get_format = get_hw_format;
        /// 告知硬件解码器,解码输出格式
        /// 这个回调函数在被调用时会给出一组当前AVCodec支持的解码格式
        /// 这个数组按解码性能从高到低排列®
        /// 开发者可以按需返回一个最合适的
        /// decode时,开发者不设置则使用av_hwdevice_ctx_create创建时指定的格式
        /// 不能设置成NULL
    }
    //codec open
    if (avcodec_open2(this->m_pCodercCtxVideo, this->m_pCodercVideo, nullptr) < 0) {
        VPLOGE("cannot open codec");
        return;
    }
    if (m_pPacket) {
        av_free(m_pPacket);
    }
    m_pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    if (this->m_pFrame) {
        av_frame_free(&this->m_pFrame);
    }
    this->m_pFrame = av_frame_alloc();

}

AVFrame* FFFFmpegDecode::getNextFrame()
{
    int ret = -1;
    AVFrame* frame = av_frame_alloc();
    while (true) {
        ret = av_read_frame(m_pFormatCtx, m_pPacket);
        if (ret != 0) {
            VPLOGE("av_read_frame end");
            break;
        }
        if (m_pPacket->stream_index != m_videoIndex) {
            av_packet_unref(m_pPacket);
            continue;
        }
        ret = avcodec_send_packet(m_pCodercCtxVideo, m_pPacket);
        if (ret == AVERROR(EAGAIN)) {
            VPLOGE("avcodec_send_packet EAGAIN");
        }
        else if (ret < 0) {
            av_packet_unref(m_pPacket);
            return nullptr;
        }
      
        
        ret = avcodec_receive_frame(m_pCodercCtxVideo, frame);
        if (ret == AVERROR(EAGAIN)) {
           continue;
        }
        VPLOGW("avcodec_receive_frame：%d", ret);
        while (ret != 0) {
            ret = avcodec_receive_frame(m_pCodercCtxVideo, frame);
        }
        //VPLOGW("获取解码数据成功：%s", av_get_pix_fmt_name(static_cast<AVPixelFormat>(frame->format)));
        VPLOGW("linesize0:%d,linesize1:%d,linesize2:%d", frame->linesize[0], frame->linesize[1], frame->linesize[2]);
        VPLOGW("width:%d,height:%d", frame->width, frame->height);
        av_packet_unref(m_pPacket);
        break;
    }
    return  frame;
}

void FFFFmpegDecode::releaseFrame(AVFrame* frame)
{
    if (frame) {
        av_frame_free(&frame);
    }
   
}
