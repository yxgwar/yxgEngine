#pragma once

#include "openGL/framebuffer.h"
#include "openGL/uniformbuffer.h"
#include "openGl/texture.h"
#include "camera.h"
#include <unordered_map>
#include <memory>

#define SHADOWMAP 7

enum class FBOType
{
    DepthMap,
    Forward,
    HDR,
    BLURH,
    BLURV,
    gBUFFER,
    SSAO,
    SSAOblur
};

class RenderContext
{
public:
    static RenderContext& GetInstance()
    {
        static RenderContext instance; // C++11保证线程安全
        return instance;
    }

    std::shared_ptr<FrameBuffer> GetFBO(FBOType type);

    void GenDepthMap(int width);
    void GenForwad(int width, int height);
    void GenHDR(int width, int height);
    void GengBuffer(int width, int height);
    void GenSSAO(int width, int height, void* noise);

    void GenUBO();
    void UpdateGlobalUBO(Camera* camera);

    // 禁用拷贝和赋值
    RenderContext(const RenderContext&) = delete;
    void operator=(const RenderContext&) = delete;

// imgui用
public:
    bool GetSSAO() const {return ssao;}
    void SetSSAO(bool flag) {ssao = flag;}

private:
    RenderContext() = default; // 私有构造函数
    ~RenderContext() = default;
    
    std::unordered_map<int, std::shared_ptr<FrameBuffer>> fboPool;

    std::unique_ptr<UniformBuffer> cameraUBO;
    std::unique_ptr<Texture> noiseTex;
private:
    bool ssao = true;
};