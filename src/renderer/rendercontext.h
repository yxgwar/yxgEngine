#pragma once

#include "openGL/framebuffer.h"
#include "openGL/uniformbuffer.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "camera.h"
#include "import/import.h"
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
    gBUFFER
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

    void GenUBO();
    void UpdateGlobalUBO(Camera* camera);

    // 禁用拷贝和赋值
    RenderContext(const RenderContext&) = delete;
    void operator=(const RenderContext&) = delete;

private:
    RenderContext() = default; // 私有构造函数
    ~RenderContext() = default;
    
    std::unordered_map<int, std::shared_ptr<FrameBuffer>> fboPool;

    std::unique_ptr<UniformBuffer> cameraUBO;
};