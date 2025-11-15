#include "FFMediaPlayer.h"
#include "FFFmpegDecode.h"
#include "FFAngleContext.h"
#ifdef __cplusplus
extern "C"
{
#endif
#include "libavutil/hwcontext_d3d11va.h"
#ifdef __cplusplus
}
#endif


FFMediaPlayer::FFMediaPlayer(const std::string& fileName) : 
    m_decodeThread(&FFMediaPlayer::decodeThreadRunLoop,this),
    m_renderAndDisplayThread(&FFMediaPlayer::renderThreadRunLoop,this)
{
    m_decode = std::make_shared<FFFFmpegDecode>(fileName);
}

FFMediaPlayer::~FFMediaPlayer()
{
    m_staus = RunningStaus::Exit;
    if (m_decodeThread.joinable()) {
        m_decodeThread.join();
    }
    if (m_renderAndDisplayThread.joinable()) {
        m_renderAndDisplayThread.join();
    }
    while (!m_frameDeque.empty())
    {
        auto frame = m_frameDeque.front();
        m_decode->releaseFrame(frame);
        m_frameDeque.pop_front();
    }
}

bool FFMediaPlayer::initEngin(HWND handle)
{
    m_handle = handle;
    m_staus = RunningStaus::Pause;

    return true;
}

bool FFMediaPlayer::setViewSize(int width, int height)
{
    m_width = width;
    m_height = height;
    return true;
}

bool FFMediaPlayer::play()
{
    m_staus = RunningStaus::Running;
    return false;
}

bool FFMediaPlayer::pasue()
{
    m_staus = RunningStaus::Pause;
    return false;
}

bool FFMediaPlayer::close()
{
    m_staus = RunningStaus::Exit;
    return false;
}

void FFMediaPlayer::renderThreadRunLoop()
{
    m_angleInit = false;
    while (m_staus != RunningStaus::Exit)
    {
        if (!m_angleInit && m_handle != nullptr && !m_angleCtx) {
            auto handle = GetDC(m_handle);
            m_angleCtx = std::make_shared<FFAngleContext>(handle);
            m_angleInit = true;
        }
        if ((m_width != 0 || m_height != 0 )&& m_angleCtx) {
            m_angleCtx->setViewSize(m_width,m_height);
        }
        if (m_staus != RunningStaus::Running) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }
        //获取队列首帧
        AVFrame* frameOperator = nullptr;
        {
            std::unique_lock<std::mutex> _(m_decodeLock);
            if (m_frameDeque.empty()) {
                continue;
            }
            frameOperator = m_frameDeque.front();
            m_frameDeque.pop_front();
        }
        
        if (frameOperator->format != AV_PIX_FMT_D3D11) {
            continue;
        }


        ID3D11Texture2D* texure = (ID3D11Texture2D*) frameOperator->data[0];
        int index = (int)frameOperator->data[1];
        D3D11_TEXTURE2D_DESC desc{};
        texure->GetDesc(&desc);
        Microsoft::WRL::ComPtr<ID3D11Device> decodeDevice;
        texure->GetDevice(decodeDevice.ReleaseAndGetAddressOf());
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> decodeCtx;
        decodeDevice->GetImmediateContext(decodeCtx.ReleaseAndGetAddressOf());

        auto texureRender = m_angleCtx->getD3D11Texture(desc.Width,desc.Height,desc.Format);
        D3D11_QUERY_DESC queryDesc;
        queryDesc.Query = D3D11_QUERY_TIMESTAMP;
        queryDesc.MiscFlags = 0;
        Microsoft::WRL::ComPtr<ID3D11Query> query;
        // 创建查询对象
        decodeDevice->CreateQuery(&queryDesc, &query);

        //共享
        Microsoft::WRL::ComPtr<IDXGIResource> d3d11Resoure;
        HANDLE sharedHandle = 0;
        texureRender->QueryInterface(__uuidof(IDXGIResource),(void**)d3d11Resoure.ReleaseAndGetAddressOf());
        auto hr = d3d11Resoure->GetSharedHandle(&sharedHandle);
        Microsoft::WRL::ComPtr<IDXGIResource> targetResource;
        decodeDevice->OpenSharedResource(sharedHandle,__uuidof(IDXGIResource),(void**)targetResource.ReleaseAndGetAddressOf());
        Microsoft::WRL::ComPtr<ID3D11Texture2D> texureTarget;
        targetResource.As(&texureTarget);
        //拷贝

        decodeCtx->CopySubresourceRegion(texureTarget.Get(),0,0,0,0,texure,index,nullptr);
        //等待
        decodeCtx->End(query.Get());
        UINT64 startTime = 0;
        while (decodeCtx->GetData(query.Get(), &startTime, sizeof(startTime), 0) != S_OK);
        //device_hwctx->unlock(device_hwctx->lock_ctx);
        //render
        m_angleCtx->renderD3D11Texture2D(texureRender);
        


        m_decode->releaseFrame(frameOperator);
        std::this_thread::sleep_for(std::chrono::microseconds(30));
    }
    m_angleCtx = nullptr;
    m_angleInit = false;

}

void FFMediaPlayer::decodeThreadRunLoop()
{
    while (m_staus != RunningStaus::Exit)
    {
        
        if (m_staus != RunningStaus::Running || !m_decode) {
            std::this_thread::sleep_for(std::chrono::microseconds(10));
            continue;
        }
        std::unique_lock<std::mutex> _(m_decodeLock);
        if (m_frameDeque.size() >= MAX_CACHE_SIZE) {
            continue;
        }
        AVFrame* frame = m_decode->getNextFrame();
        m_frameDeque.push_back(frame);

    }

}
