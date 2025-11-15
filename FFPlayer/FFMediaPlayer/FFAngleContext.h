#pragma once
#include "angleloader.h"
#include"gles_loader_autogen.h"
#include "egl_loader_autogen.h"
#include <d3d11.h>
#include "wrl/client.h"
enum class RenderBackGround
{
    USING_D3D11,
    USING_VULCAN,
    USING_D3D12,
    USING_D3D11ON12,
    USING_D3D9
};
class FFAngleContext {
public:
    FFAngleContext(HDC handle);
    ~FFAngleContext();
public:
    void test();
    bool renderD3D11Texture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texure);
    void setViewSize(int width,int height);
    Microsoft::WRL::ComPtr<ID3D11Texture2D> getD3D11Texture(int width, int height, DXGI_FORMAT fmt);
private: 
    void loadAngle();
    void initCtx(HDC handle);
    bool initShader();
    bool dumpNV12TextureToFile(Microsoft::WRL::ComPtr<ID3D11Texture2D> nv12Texture, const std::string& filename);
private:
    Microsoft::WRL::ComPtr<ID3D11Texture2D> m_texureNV12;
    int m_index = 0;
    EGLDisplay m_eglDisplay = EGL_NO_DISPLAY;
    EGLContext m_eglContext = EGL_NO_CONTEXT;
    EGLSurface m_eglSurface = EGL_NO_SURFACE;
    ID3D11Device* m_d3d11Device = nullptr;
    uint32_t m_vertexbuffer = 0;
    uint32_t m_uvbuffer = 0;
    uint32_t m_program = 0;
    uint32_t m_textureUniformY = 0;
    uint32_t m_textureUniformUV = 0;
    uint32_t m_textureUniformV = 0;
    uint32_t m_idY = 0;
    uint32_t m_idUV = 0;
    uint32_t m_idV = 0;
    uint32_t m_width = 0;
    uint32_t m_height = 0;
};