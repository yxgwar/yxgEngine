#include "renderpipeline.h"
#include "scene.h"

void RenderPipeline::AddPass(std::unique_ptr<IRenderPass> pass)
{
    m_passes.emplace_back(std::move(pass));
}

void RenderPipeline::Execute(Scene &scene, RenderContext& context)
{
    for(auto& pass : m_passes) 
    {
        pass->Execute(scene, context);
    }
}

void RenderPipeline::UpdateGlobalUBO(Camera *camera, RenderContext& context)
{
    context.UpdateGlobalUBO(camera);
}

PrePass::PrePass(RenderContext &context)
{
    context.GenUBO();
}

ShadowMapPass::ShadowMapPass(RenderContext& context)
{
    context.GenDepthMap(4096);
}

void ShadowMapPass::Execute(Scene &scene, RenderContext &context)
{
    auto light = scene.GetMainLight();
    auto fbo = context.GetFBO(FBOType::DepthMap);
    fbo->bind();
    glClearDepthf(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    if(light)
    {
        auto lc = light->GetComponent<CameraComponent>();
        glm::mat4 vp = lc->projection * std::get<glm::mat4>(lc->view);
        for(auto entity : scene.GetEntities())
        {
            if(auto render = entity->GetComponent<RenderComponent>())
                render->RenderDepth(vp);
        }
    }
    fbo->unbind();
}

ForwardPass::ForwardPass(RenderContext &context, int width, int height)
{
    context.GenForwad(width, height);
}

void ForwardPass::Execute(Scene &scene, RenderContext &context)
{
    auto fbo = context.GetFBO(FBOType::Forward);
    fbo->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    auto camera = scene.GetCamera();

    auto light = scene.GetMainLight();
    for(auto entity : scene.GetEntities())
    {
        if(auto render = entity->GetComponent<RenderComponent>())
        {
            render->Render(*camera, light);
        }
    }
    fbo->unbind();
}

void PostProcessPass::Execute(Scene &scene, RenderContext &context)
{
    auto fbo = context.GetFBO(FBOType::Forward);
    fbo->bindTexture();
    ImGuiRenderer::imguiF->bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderQuad::DrawwithShader(*Import::ShaderPool["PostProcess"]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
