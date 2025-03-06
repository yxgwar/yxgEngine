#pragma once

#include "rendercontext.h"
#include "RenderQuad.h"
#include "ImGuiRenderer.h"
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

class PrePass: public IRenderPass
{
public:
    PrePass(RenderContext& context);

    void Execute(Scene& scene, RenderContext& context) override {}
};

class ShadowMapPass : public IRenderPass
{
public:
    ShadowMapPass(RenderContext& context);

    void Execute(Scene& scene, RenderContext& context) override;
};

class ForwardPass: public IRenderPass
{
public:
    ForwardPass(RenderContext& context, bool hdr = false, int width = 1920, int height = 1080);

    void Execute(Scene& scene, RenderContext& context) override;
private:
    bool m_hdr;
};

class PostProcessPass: public IRenderPass
{
public:
    PostProcessPass(bool hdr = false);

    void Execute(Scene& scene, RenderContext& context) override;
private:
    bool m_hdr;
};