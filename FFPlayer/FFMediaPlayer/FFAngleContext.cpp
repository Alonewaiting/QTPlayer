#include "FFAngleContext.h"

#include <vector>

#include <mutex>
#include <wrl/client.h>
#include "angleloader.h"
#include <dxgi.h>
#include <iostream>
#pragma comment(lib, "dxgi.lib")
std::vector<DXGI_ADAPTER_DESC> getAdapters() {
    std::vector<DXGI_ADAPTER_DESC>mAdapterDescs;
    Microsoft::WRL::ComPtr<IDXGIFactory1> factory;
    if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
        for (UINT i = 0; SUCCEEDED(factory->EnumAdapters(i, &adapter)); i++) {
            DXGI_ADAPTER_DESC desc;
            if (SUCCEEDED(adapter->GetDesc(&desc))) {
                mAdapterDescs.emplace_back(desc);
            }
        }
    }
    return mAdapterDescs;
}
bool FFAngleContext::dumpNV12TextureToFile(Microsoft::WRL::ComPtr<ID3D11Texture2D> nv12Texture, const std::string& filename)
{
    if (!nv12Texture || !m_d3d11Device) {
        std::cout << "NV12纹理或D3D11设备为空" << std::endl;
        return false;
    }

    // 获取纹理描述
    D3D11_TEXTURE2D_DESC textureDesc;
    nv12Texture->GetDesc(&textureDesc);

    // 检查格式是否为NV12
    if (textureDesc.Format != DXGI_FORMAT_NV12) {
        std::cout << "纹理格式不是NV12，当前格式: " << textureDesc.Format << std::endl;
        return false;
    }

    std::cout << "NV12纹理信息 - 宽度: " << textureDesc.Width
        << ", 高度: " << textureDesc.Height
        << ", 格式: " << textureDesc.Format << std::endl;

    // 创建staging纹理用于读取
    D3D11_TEXTURE2D_DESC stagingDesc = {};
    stagingDesc.Width = textureDesc.Width;
    stagingDesc.Height = textureDesc.Height;
    stagingDesc.MipLevels = 1;
    stagingDesc.ArraySize = 1;
    stagingDesc.Format = textureDesc.Format; // 保持NV12格式
    stagingDesc.SampleDesc.Count = 1;
    stagingDesc.SampleDesc.Quality = 0;
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.BindFlags = 0;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingDesc.MiscFlags = 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTexture;
    HRESULT hr = m_d3d11Device->CreateTexture2D(&stagingDesc, nullptr, &stagingTexture);
    if (FAILED(hr)) {
        std::cout << "创建staging纹理失败，错误码: 0x" << std::hex << hr << std::endl;
        return false;
    }

    // 获取立即执行上下文
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    m_d3d11Device->GetImmediateContext(&deviceContext);
    if (!deviceContext) {
        std::cout << "获取设备上下文失败" << std::endl;
        return false;
    }

    // 将数据拷贝到staging纹理
    deviceContext->CopyResource(stagingTexture.Get(), nv12Texture.Get());

    // 映射staging纹理以读取数据
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    hr = deviceContext->Map(stagingTexture.Get(), 0, D3D11_MAP_READ, 0, &mappedResource);
    if (FAILED(hr)) {
        std::cout << "映射staging纹理失败，错误码: 0x" << std::hex << hr << std::endl;
        return false;
    }

    // 计算NV12数据大小
    // NV12格式: Y平面 = width * height, UV平面 = width * height / 2
    size_t yPlaneSize = textureDesc.Width * textureDesc.Height;
    size_t uvPlaneSize = textureDesc.Width * textureDesc.Height / 2;
    size_t totalSize = yPlaneSize + uvPlaneSize;

    std::cout << "NV12数据大小 - Y平面: " << yPlaneSize
        << " 字节, UV平面: " << uvPlaneSize
        << " 字节, 总计: " << totalSize << " 字节" << std::endl;
    std::cout << "映射行间距: " << mappedResource.RowPitch << " 字节" << std::endl;

    // 创建输出文件
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile.is_open()) {
        std::cout << "无法创建输出文件: " << filename << std::endl;
        deviceContext->Unmap(stagingTexture.Get(), 0);
        return false;
    }

    try {
        // 写入Y平面数据
        uint8_t* mappedData = static_cast<uint8_t*>(mappedResource.pData);
        for (UINT row = 0; row < textureDesc.Height; row++) {
            outFile.write(reinterpret_cast<char*>(mappedData + row * mappedResource.RowPitch),
                textureDesc.Width);
        }

        // 写入UV平面数据 (NV12中UV是交错的，从height/2行开始)
        uint8_t* uvData = mappedData + (textureDesc.Height * mappedResource.RowPitch);
        for (UINT row = 0; row < textureDesc.Height / 2; row++) {
            outFile.write(reinterpret_cast<char*>(uvData + row * mappedResource.RowPitch),
                textureDesc.Width); // UV交错，所以是width字节
        }

        std::cout << "NV12数据已成功写入文件: " << filename << std::endl;
        std::cout << "文件大小: " << outFile.tellp() << " 字节" << std::endl;

    }
    catch (const std::exception& e) {
        std::cout << "写入文件时发生异常: " << e.what() << std::endl;
        deviceContext->Unmap(stagingTexture.Get(), 0);
        outFile.close();
        return false;
    }

    // 清理
    deviceContext->Unmap(stagingTexture.Get(), 0);
    outFile.close();

    return true;
}

FFAngleContext::FFAngleContext(HDC handle)
{

    loadAngle();
    initCtx(handle);
    initShader();

}

FFAngleContext::~FFAngleContext()
{
    // 1. 确保 EGL 上下文是当前
    if (m_eglDisplay != EGL_NO_DISPLAY && m_eglContext != EGL_NO_CONTEXT)
        eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);

    // 2. 销毁 GL 相关对象
    if (m_program)
    {
        glDeleteProgram(m_program);
        m_program = 0;
    }

    // 3. 销毁 EGLSurface
    if (m_eglDisplay != EGL_NO_DISPLAY && m_eglSurface != EGL_NO_SURFACE)
    {
        eglDestroySurface(m_eglDisplay, m_eglSurface);
        m_eglSurface = EGL_NO_SURFACE;
    }

    // 4. 销毁 EGLContext
    if (m_eglDisplay != EGL_NO_DISPLAY && m_eglContext != EGL_NO_CONTEXT)
    {
        eglDestroyContext(m_eglDisplay, m_eglContext);
        m_eglContext = EGL_NO_CONTEXT;
    }

    // 5. 释放 D3D11 设备引用
    if (m_d3d11Device)
    {
        m_d3d11Device->Release();
        m_d3d11Device = nullptr;
    }

    // 6. 终止 EGL 显示连接
    if (m_eglDisplay != EGL_NO_DISPLAY)
    {
        eglTerminate(m_eglDisplay);
        m_eglDisplay = EGL_NO_DISPLAY;
    }
}

void FFAngleContext::test()
{
       // 创建简单的着色器程序
    const char* vertexShaderSource = R"(
        attribute vec4 aPosition;
        attribute vec2 aTexCoord;
        varying vec2 vTexCoord;
        void main() {
            gl_Position = aPosition;
            vTexCoord = aTexCoord;
        }
    )";

    const char* fragmentShaderSource = R"(
        precision mediump float;
        uniform sampler2D uTexture;
        varying vec2 vTexCoord;
        void main() {
            gl_FragColor = texture2D(uTexture, vTexCoord);
        }
    )";

    // 编译顶点着色器
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // 检查顶点着色器编译状态
    GLint compiled;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            std::vector<char> infoLog(infoLen);
            glGetShaderInfoLog(vertexShader, infoLen, NULL, infoLog.data());
            std::cout << "顶点着色器编译错误: " << infoLog.data() << std::endl;
        }
        glDeleteShader(vertexShader);
        return;
    }

    // 编译片段着色器
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // 检查片段着色器编译状态
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLen = 0;
        glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            std::vector<char> infoLog(infoLen);
            glGetShaderInfoLog(fragmentShader, infoLen, NULL, infoLog.data());
            std::cout << "片段着色器编译错误: " << infoLog.data() << std::endl;
        }
        glDeleteShader(fragmentShader);
        return;
    }

    // 创建程序对象
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // 检查程序链接状态
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        if (infoLen > 1) {
            std::vector<char> infoLog(infoLen);
            glGetProgramInfoLog(program, infoLen, NULL, infoLog.data());
            std::cout << "程序链接错误: " << infoLog.data() << std::endl;
        }
        glDeleteProgram(program);
        return;
    }

    // 删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // 创建纯色纹理 (红色)
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // 创建256x256的红色纹理数据
    const int texWidth = 256;
    const int texHeight = 256;
    std::vector<GLubyte> textureData(texWidth * texHeight * 4); // RGBA
    
    for (int y = 0; y < texHeight; y++) {
        for (int x = 0; x < texWidth; x++) {
            int index = (y * texWidth + x) * 4;
            textureData[index] = 255;     // R
            textureData[index + 1] = 0;   // G
            textureData[index + 2] = 0;   // B
            textureData[index + 3] = 255; // A
        }
    }

    // 上传纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());

    // 创建顶点数据 (全屏四边形)
    GLfloat vertices[] = {
        // 位置          // 纹理坐标
        -1.0f,  1.0f,   0.0f, 0.0f,  // 左上角
        -1.0f, -1.0f,   0.0f, 1.0f,  // 左下角
         1.0f,  1.0f,   1.0f, 0.0f,  // 右上角
         1.0f, -1.0f,   1.0f, 1.0f   // 右下角
    };

    // 创建顶点缓冲对象
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 获取着色器中的属性位置
    GLint positionLoc = glGetAttribLocation(program, "aPosition");
    GLint texCoordLoc = glGetAttribLocation(program, "aTexCoord");

    // 清除屏幕
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // 使用着色器程序
    glUseProgram(program);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    GLint textureLoc = glGetUniformLocation(program, "uTexture");
    glUniform1i(textureLoc, 0);

    // 设置顶点属性
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
    
    glEnableVertexAttribArray(texCoordLoc);
    glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // 绘制四边形
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // 交换缓冲区以显示结果
    eglSwapBuffers(m_eglDisplay, m_eglSurface);

    // 清理
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &texture);
    glDeleteProgram(program);
    
    std::cout << "纯色纹理渲染完成!" << std::endl;
}
#define GET_STR(x) #x

static const char* fragNV12 =
"#extension GL_OES_EGL_image_external : require      \n"
"#ifdef GL_ES									   \n"
"precision highp  float;						   \n"
"#else											   \n"
"#define highp									   \n"
"#define mediump								   \n"
"#define lowp									   \n"
"#endif											   \n"
"uniform samplerExternalOES textureY; 										\n"
"uniform samplerExternalOES textureUV; 										\n"
"varying vec2 texcoordOut; 											\n"
"uniform int debugMode;  // 调试模式: 0-正常, 1-只显示Y, 2-只显示U, 3-只显示V\n"
"																	\n"
"void main()										 \n"
"{													 \n"
"    highp vec3 yuv;								 \n"
"    highp vec3 rgb;								 \n"
"    \n"
"    // 从NV12纹理采样Y和UV分量\n"
"    yuv.x = texture2D(textureY, texcoordOut).r;      // Y分量\n"
"    yuv.y = texture2D(textureUV, texcoordOut).r;     // U分量\n"
"    yuv.z = texture2D(textureUV, texcoordOut).g;     // V分量\n"
"    \n"
"    // 调试模式：单独显示各分量\n"
"    if (debugMode == 1) {\n"
"        gl_FragColor = vec4(yuv.x, yuv.x, yuv.x, 1.0);  // 只显示Y分量\n"
"        return;\n"
"    } else if (debugMode == 2) {\n"
"        gl_FragColor = vec4(yuv.y, yuv.y, yuv.y, 1.0);  // 只显示U分量\n"
"        return;\n"
"    } else if (debugMode == 3) {\n"
"        gl_FragColor = vec4(yuv.z, yuv.z, yuv.z, 1.0);  // 只显示V分量\n"
"        return;\n"
"    }\n"
"    \n"
"    // BT.709标准的YUV到RGB转换\n"
"    // 首先减去偏移量\n"
"    yuv.x = yuv.x - 0.0625;      // Y分量偏移：16/256\n"
"    yuv.y = yuv.y - 0.5;         // U分量偏移\n"
"    yuv.z = yuv.z - 0.5;         // V分量偏移\n"
"    \n"
"    // 使用BT.709标准的转换矩阵\n"
"    rgb.r = yuv.x + 1.5748 * yuv.z;                    // R\n"
"    rgb.g = yuv.x - 0.1873 * yuv.y - 0.4681 * yuv.z;  // G  \n"
"    rgb.b = yuv.x + 1.8556 * yuv.y;                    // B\n"
"    \n"
"    // 确保颜色值在有效范围内\n"
"    rgb = clamp(rgb, 0.0, 1.0);\n"
"    \n"
"    gl_FragColor = vec4(rgb, 1.0); 		 \n"
"}											   		 \n"
;

static const char* vertexShaderSource =
"attribute vec3 position;						  \n"
"attribute vec2 texcoord;						  \n"
"varying vec2 texcoordOut;			     		  \n"
"												  \n"
"void main()									  \n"
"{												  \n"
"	texcoordOut = texcoord;         			  \n"
"	gl_Position = vec4(position, 1.0); \n"  
"}												  \n"
;



#if TEXTURE_ROTATE  
static const GLfloat vertexVertices[] = {
    -1.0f, -0.5f,
     0.5f, -1.0f,
    -0.5f,  1.0f,
     1.0f,  0.5f,
};
#else  
static const GLfloat vertexVertices[] = {
    1.0f, 1.0f, 0.0f,
    1.0f, -1.0f,  0.0f,
    -1.0f, -1.0f,  0.0f,
    -1.0f,  1.0f,  0.0f
};
#endif  

#if TEXTURE_HALF  
static const GLfloat textureVertices[] = {
    0.0f,  1.0f,
    0.5f,  1.0f,
    0.0f,  0.0f,
    0.5f,  0.0f,
};
#else  
static const GLfloat textureVertices[] = {
    1.0f,  1 - 1.0f,
    1.0f,  1 - 0.0f,
    0.0f,  1 - 0.0f,
    0.0f,  1 - 1.0f,
};
#endif 



bool FFAngleContext::renderD3D11Texture2D(Microsoft::WRL::ComPtr<ID3D11Texture2D> texure) {
    // 获取纹理描述
    D3D11_TEXTURE2D_DESC desc;
    texure->GetDesc(&desc);
    
    // 添加调试信息
    std::cout << "纹理格式: " << desc.Format << ", 大小: " << desc.Width << "x" << desc.Height << std::endl;
    
    // 创建Y平面EGLImage
    const EGLint yAttribs[] = { EGL_D3D11_TEXTURE_PLANE_ANGLE, 0, EGL_NONE };
    EGLImage yImage = eglCreateImageKHR(m_eglDisplay, EGL_NO_CONTEXT, EGL_D3D11_TEXTURE_ANGLE,
        static_cast<EGLClientBuffer>(texure.Get()), yAttribs);
    
    GLuint yTexture;
    glGenTextures(1, &yTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, yTexture);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, yImage);

    // 创建UV平面EGLImage
    const EGLint uvAttribs[] = { EGL_D3D11_TEXTURE_PLANE_ANGLE, 1, EGL_NONE };
    EGLImage uvImage = eglCreateImageKHR(m_eglDisplay, EGL_NO_CONTEXT, EGL_D3D11_TEXTURE_ANGLE,
        static_cast<EGLClientBuffer>(texure.Get()), uvAttribs);

    GLuint uvTexture;
    glGenTextures(1, &uvTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, uvTexture);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glEGLImageTargetTexture2DOES(GL_TEXTURE_EXTERNAL_OES, uvImage);

    // 清除屏幕
    glViewport(0,0,m_width,m_height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
  
    // 使用着色器程序
    glUseProgram(m_program);

    GLfloat vertices[] = {
        // 位置          // 纹理坐标
        -1.0f,  1.0f,   0.0f, 0.0f,  // 左上角
        -1.0f, -1.0f,   0.0f, 1.0f,  // 左下角
         1.0f,  1.0f,   1.0f, 0.0f,  // 右上角
         1.0f, -1.0f,   1.0f, 1.0f   // 右下角
    };

    // 创建顶点缓冲对象
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 绑定Y纹理到纹理单元0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, yTexture);
    GLint textureLocY = glGetUniformLocation(m_program, "textureY");
    glUniform1i(textureLocY, 0);

    // 绑定UV纹理到纹理单元1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, uvTexture);
    GLint textureLocUV = glGetUniformLocation(m_program, "textureUV");
    glUniform1i(textureLocUV, 1);

    // 设置调试模式：0-正常渲染，1-只显示Y，2-只显示U，3-只显示V
    GLint debugModeLoc = glGetUniformLocation(m_program, "debugMode");
    glUniform1i(debugModeLoc, 0);  // 默认正常渲染

    // 获取着色器中的属性位置
    GLint positionLoc = glGetAttribLocation(m_program, "position");
    GLint texCoordLoc = glGetAttribLocation(m_program, "texcoord");
    
    // 设置顶点属性
    glEnableVertexAttribArray(positionLoc);
    glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);

    glEnableVertexAttribArray(texCoordLoc);
    glVertexAttribPointer(texCoordLoc, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    // 绘制四边形
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // 交换缓冲区以显示结果
    eglSwapBuffers(m_eglDisplay, m_eglSurface);

    // 清理
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, &yTexture);
    glDeleteTextures(1, &uvTexture);
    eglDestroyImageKHR(m_eglDisplay, yImage);
    eglDestroyImageKHR(m_eglDisplay, uvImage);

    return true;
}

void FFAngleContext::setViewSize(int width, int height)
{
    m_width = width;
    m_height = height;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> FFAngleContext::getD3D11Texture(int width,int height, DXGI_FORMAT fmt) {
    if (m_texureNV12) {
        return m_texureNV12;
    }
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = fmt;
    desc.SampleDesc.Count = 1;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
    desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
    auto hr = m_d3d11Device->CreateTexture2D(&desc,nullptr,m_texureNV12.ReleaseAndGetAddressOf());
    if (FAILED(hr)) {
        return nullptr;
    }
    return m_texureNV12;
    
}

void FFAngleContext::loadAngle()
{
    wchar_t exeFullPath[MAX_PATH]; // Full path 
    std::string strPath = "";

    GetModuleFileNameW(NULL, exeFullPath, MAX_PATH);
    char CharString[MAX_PATH];
    size_t convertedChars = 0;
    wcstombs_s(&convertedChars, CharString, MAX_PATH, exeFullPath, _TRUNCATE);

    strPath = (std::string)CharString;    // Get full path of the file 
    int pos = strPath.find_last_of('\\', strPath.length());
    auto filePath = strPath.substr(0, pos);  // Return the directory without the file name 
    const std::string finKey = "FFPlayer";
    int pos2 = filePath.find(finKey);

    auto workSpace = strPath.substr(0, pos2);
    auto libEGLPath = workSpace + "FFPlayer\\FFPlayer\\3rd\\angle\\libs\\x64\\libEGL.dll";
    auto libGLESPath = workSpace + "FFPlayer\\FFPlayer\\3rd\\angle\\libs\\x64\\libGLESv2.dll";
    angle::loadEGLLibrary(libEGLPath.c_str());
    angle::loadGLESLibrary(libGLESPath.c_str());
}

void FFAngleContext::initCtx(HDC handle)
{
    m_index = 0;
    EGLNativeDisplayType dpy = handle ? handle : EGL_DEFAULT_DISPLAY;
    std::vector<EGLAttrib> attribList;
    attribList.reserve(30);
    auto angle_type = EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE;
    auto background = RenderBackGround::USING_D3D11;
    switch (background)
    {
    case RenderBackGround::USING_D3D11:
        angle_type = EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE;
        break;
    case RenderBackGround::USING_D3D11ON12:
        break;
    case RenderBackGround::USING_D3D9:
        angle_type = EGL_PLATFORM_ANGLE_TYPE_D3D11_ANGLE;
        break;
    case RenderBackGround::USING_VULCAN:
        angle_type = EGL_PLATFORM_ANGLE_TYPE_VULKAN_ANGLE;
        break;
    default:
        break;
    }

    auto device_type = EGL_PLATFORM_ANGLE_DEVICE_TYPE_HARDWARE_ANGLE;
    auto adapter = getAdapters();

    if (m_index >= adapter.size()) {
        std::cout << "index out of range" << std::endl;
        return;
    }
    attribList.push_back(EGL_PLATFORM_ANGLE_TYPE_ANGLE);
    attribList.push_back(angle_type);
    attribList.push_back(EGL_PLATFORM_ANGLE_DEVICE_TYPE_ANGLE);
    attribList.push_back(device_type);
    attribList.push_back(EGL_PLATFORM_ANGLE_MAX_VERSION_MAJOR_ANGLE);
    attribList.push_back(EGL_DONT_CARE);
    attribList.push_back(EGL_PLATFORM_ANGLE_MAX_VERSION_MINOR_ANGLE);
    attribList.push_back(EGL_DONT_CARE);
    if (background == RenderBackGround::USING_D3D11) {
        attribList.push_back(EGL_PLATFORM_ANGLE_D3D_LUID_HIGH_ANGLE);  //显卡LUID
        attribList.push_back(adapter[m_index].AdapterLuid.HighPart);
        attribList.push_back(EGL_PLATFORM_ANGLE_D3D_LUID_LOW_ANGLE);
        attribList.push_back(adapter[m_index].AdapterLuid.LowPart);
    }

    attribList.push_back(EGL_NONE);
    if (m_eglDisplay == EGL_NO_DISPLAY)
    {
        if (eglGetPlatformDisplay) {
            m_eglDisplay = eglGetPlatformDisplay(EGL_PLATFORM_ANGLE_ANGLE, dpy, attribList.data());
        }
    }
    if (m_eglDisplay == EGL_NO_DISPLAY) {
        std::cout << "egl display create failded" << std::endl;
        return;
    }
    // 初始化EGL
    EGLint majorVersion, minorVersion;
    if (!eglInitialize(m_eglDisplay, &majorVersion, &minorVersion))
    {
        // 处理错误
        std::cout << "eglInitialize failded" << std::endl;
        return;
    }
    // 设置EGL配置属性
    EGLint configAttributes[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_SAMPLE_BUFFERS, 0,
        EGL_NONE
    };
    // 选择EGL配置
    EGLConfig eglConfig;
    EGLint numConfigs;
    if (!eglChooseConfig(m_eglDisplay, configAttributes, &eglConfig, 1, &numConfigs))
    {
        std::cout << "eglChooseConfig  failded" << std::endl;
        return;
    }
    // 创建EGLContext
    EGLint contextAttributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };
    m_eglContext = eglCreateContext(m_eglDisplay, eglConfig, EGL_NO_CONTEXT, contextAttributes);
    if (m_eglContext == EGL_NO_CONTEXT)
    {
        std::cout << "eglCreateContext  failded" << std::endl;
        return;
    }

    // 创建EGLSurface
    EGLNativeWindowType nativeWindow = WindowFromDC(dpy);
    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, eglConfig, nativeWindow, nullptr);
    if (m_eglSurface == EGL_NO_SURFACE)
    {
        std::cout << "eglCreateWindowSurface  failded" << std::endl;
        return;
    }

    // 绑定EGLContext和EGLSurface
    if (!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext))
    {
        std::cout << "eglMakeCurrent failed" << std::endl;
        return;
    }
    
    //// ✅ 检查GL_OES_EGL_image_external扩展支持
    //const char* extensions = glGetString(GL_EXTENSIONS);
    //if (extensions) {
    //    std::cout << "GL Extensions: " << extensions << std::endl;
    //    if (strstr(extensions, "GL_OES_EGL_image_external") == nullptr) {
    //        std::cout << "WARNING: GL_OES_EGL_image_external not supported!" << std::endl;
    //    }
    //}
    EGLDeviceEXT device = EGL_NO_DEVICE_EXT;
    EGLAttrib result = 0;
    eglQueryDisplayAttribEXT(m_eglDisplay, EGL_DEVICE_EXT, &result);
    device = reinterpret_cast<EGLDeviceEXT>(result);
    if (eglQueryDeviceAttribEXT(device, EGL_D3D11_DEVICE_ANGLE, &result)) {
        m_d3d11Device = reinterpret_cast<ID3D11Device*>(result);
        m_d3d11Device->AddRef();
    }
    else {
        std::cout << "eglQueryDeviceAttribEXT  failded" << std::endl;
    }
}

bool FFAngleContext::initShader()
{
    GLint vertCompiled, fragCompiled, linked;

    GLint v, f;
    const char* vs, * fs;
    //Shader: step1  
    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    //Shader: step2  
    glShaderSource(v, 1, &vertexShaderSource, NULL);
    glShaderSource(f, 1, &fragNV12, NULL);
    //Shader: step3  
    glCompileShader(v);
    //Debug  
    glGetShaderiv(v, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(v, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(v, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        std::printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(f, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(f, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        std::printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    //Program: Step1  
    m_program = glCreateProgram();
    //Program: Step2  
    glAttachShader(m_program, v);
    glAttachShader(m_program, f);

    /* glBindAttribLocation(p, ATTRIB_VERTEX, "vertexIn");
     glBindAttribLocation(p, ATTRIB_TEXTURE, "textureIn"); */
     //Program: Step3  

    glLinkProgram(m_program);
    //Debug  
    glGetShaderiv(m_program, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(m_program, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(m_program, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        std::printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    return true;
}