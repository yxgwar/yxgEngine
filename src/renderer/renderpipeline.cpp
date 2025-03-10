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

ForwardPass::ForwardPass(RenderContext &context, bool hdr, int width, int height)
    :m_hdr(hdr)
{
    if(hdr)
        context.GenHDR(width, height);
    else
        context.GenForwad(width, height);
}

void ForwardPass::Execute(Scene &scene, RenderContext &context)
{
    std::shared_ptr<FrameBuffer> fbo;
    if(m_hdr)
        fbo = context.GetFBO(FBOType::HDR);
    else
        fbo = context.GetFBO(FBOType::Forward);
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

GBufferPass::GBufferPass(RenderContext &context, int width, int height)
{
    context.GengBuffer(width, height);
}

void GBufferPass::Execute(Scene &scene, RenderContext &context)
{
    auto fbo = context.GetFBO(FBOType::gBUFFER);
    fbo->bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto light = scene.GetMainLight();
    auto lc = light->GetComponent<CameraComponent>();
    glm::mat4 vp = lc->projection * std::get<glm::mat4>(lc->view);

    for(auto entity : scene.GetEntities())
    {
        if(auto render = entity->GetComponent<RenderComponent>())
            render->RendergBuffer(vp, light);
    }
    fbo->unbind();
}

void LightProcessPass::Execute(Scene &scene, RenderContext &context)
{
    auto fbo = context.GetFBO(FBOType::gBUFFER);
    fbo->bindTexture();

    ImGuiRenderer::imguiF->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = Import::GetShader("deffer");
    if(shader)
    {
        shader->use();
        shader->setVec3("viewPos", scene.GetCamera()->getPosition());
        auto light = scene.GetMainLight();
        shader->setVec3("lightPos", light->GetComponent<TransformComponent>()->position);
        shader->setVec3("lightColor", light->GetComponent<LightComponent>()->color);
    
        RenderQuad::DrawwithShader(*shader);
    
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else
    {
        std::cout << "deffer shader get error!" << std::endl;
    }
}

PostProcessPass::PostProcessPass(bool hdr)
    :m_hdr(hdr)
{
}

void PostProcessPass::Execute(Scene &scene, RenderContext &context)
{
    std::shared_ptr<FrameBuffer> fbo;
    if(m_hdr)
    {
        fbo = context.GetFBO(FBOType::HDR);
        fbo->bindSingleTexture(1);

        bool horizontal = true;
        int amount = 10;
        auto blurShader = Import::GetShader("blur");
        if(blurShader)
        {
            blurShader->use();
            auto blurh = context.GetFBO(FBOType::BLURH);
            auto blurv = context.GetFBO(FBOType::BLURV);
            for (int i = 0; i <= amount; i++)
            {
                if(horizontal)
                    blurh->bind();
                else
                    blurv->bind();
                blurShader->setInt("horizontal", horizontal);
                RenderQuad::DrawwithShader(*blurShader);
                if(horizontal)
                    blurh->bindTexture();
                else
                    blurv->bindTexture();
                
                horizontal = !horizontal;
            }
            ImGuiRenderer::imguiF->bind();
    
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
            fbo->bindSingleTexture(0);
            if(!horizontal)
                blurh->bindTexture(1);
            else
                blurv->bindTexture(1);
    
            auto hdrShader = Import::GetShader("HDR");
            if(hdrShader)
                RenderQuad::DrawwithShader(*hdrShader);
            else
                std::cout << "hdr shader get error!" << std::endl;
        }
        else
        {
            std::cout << "blur shader get error!" << std::endl;
        }
    }
    else
    {
        fbo = context.GetFBO(FBOType::Forward);
        fbo->bindTexture();
        ImGuiRenderer::imguiF->bind();

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto postShader = Import::GetShader("PostProcess");
        if(postShader)
            RenderQuad::DrawwithShader(*postShader);
        else
            std::cout << "PostProcess shader get error!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}