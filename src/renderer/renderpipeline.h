#pragma once

#include "rendercontext.h"

class Scene;

// 基础渲染Pass接口
class IRenderPass
{
public:
    virtual void Execute(Scene& scene, RenderContext& context) = 0;
    virtual ~IRenderPass() = default;
};

// 定义渲染模式枚举
enum class RenderMode
{
    Forward,
    Deferred
};

// 渲染管线管理器
class RenderPipeline
{
public:
    void AddCommonPass(std::unique_ptr<IRenderPass> pass)       {m_commonPasses.emplace_back(std::move(pass));}
    void AddForwardPass(std::unique_ptr<IRenderPass> pass)      {m_forwardPasses.emplace_back(std::move(pass));}
    void AddDeferredPass(std::unique_ptr<IRenderPass> pass)     {m_deferredPasses.emplace_back(std::move(pass));}
    void AddPostProcessPass(std::unique_ptr<IRenderPass> pass)  {m_postPasses.emplace_back(std::move(pass));}
    void Execute(Scene& scene, RenderContext& context);

    void UpdateGlobalUBO(Camera* camera, RenderContext& context);

    RenderMode GetRenderMode() { return m_currentMode; }
    void SetRenderMode(RenderMode mode) { m_currentMode = mode;}
private:
    std::vector<std::unique_ptr<IRenderPass>> m_commonPasses;   // 公共Pass
    std::vector<std::unique_ptr<IRenderPass>> m_forwardPasses;  // 正向专用
    std::vector<std::unique_ptr<IRenderPass>> m_deferredPasses; // 延迟专用
    std::vector<std::unique_ptr<IRenderPass>> m_postPasses;     // 后处理pass
    RenderMode m_currentMode = RenderMode::Deferred;
};

// 预处理，创建相关实例
class PrePass: public IRenderPass
{
public:
    PrePass(RenderContext& context);

    void Execute(Scene& scene, RenderContext& context) override {}
};

// 阴影映射
class ShadowMapPass : public IRenderPass
{
public:
    ShadowMapPass(RenderContext& context);

    void Execute(Scene& scene, RenderContext& context) override;
};

// 前向渲染
class ForwardPass: public IRenderPass
{
public:
    ForwardPass(RenderContext& context, int width = 1920, int height = 1080);

    void Execute(Scene& scene, RenderContext& context) override;
};

// gBuffer
class GBufferPass : public IRenderPass
{
public:
    GBufferPass(RenderContext& context, int width = 1920, int height = 1080);

    void Execute(Scene& scene, RenderContext& context) override;
};

// 延迟渲染的光照环节
class LightProcessPass: public IRenderPass
{
public:
    void Execute(Scene& scene, RenderContext& context) override;
};

// 后处理
class PostProcessPass: public IRenderPass
{
public:
    void Execute(Scene& scene, RenderContext& context) override;
};

class SSAOPass: public IRenderPass
{
public:
    SSAOPass(RenderContext& context, int width = 1920, int height = 1080);

    void Execute(Scene& scene, RenderContext& context) override;
};