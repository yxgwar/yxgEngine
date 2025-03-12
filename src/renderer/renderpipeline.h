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

// 渲染管线管理器
class RenderPipeline
{
public:
    void AddPass(std::unique_ptr<IRenderPass> pass);
    void Execute(Scene& scene, RenderContext& context);

    void UpdateGlobalUBO(Camera* camera, RenderContext& context);
private:
    std::vector<std::unique_ptr<IRenderPass>> m_passes;
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
    ForwardPass(RenderContext& context, bool hdr = false, int width = 1920, int height = 1080);

    void Execute(Scene& scene, RenderContext& context) override;
private:
    bool m_hdr;
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
    PostProcessPass(bool hdr = false);

    void Execute(Scene& scene, RenderContext& context) override;
private:
    bool m_hdr;
};

class SSAOPass: public IRenderPass
{
public:
    SSAOPass(RenderContext& context, int width = 1920, int height = 1080);

    void Execute(Scene& scene, RenderContext& context) override;
};