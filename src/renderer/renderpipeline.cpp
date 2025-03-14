#include "renderpipeline.h"
#include "scene.h"
#include "RenderQuad.h"
#include "imguiRenderer/ImGuiRenderer.h"
#include "import/import.h"
#include "log.h"
#include <random>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

void RenderPipeline::Execute(Scene &scene, RenderContext &context)
{
    for(auto& pass : m_commonPasses) 
        pass->Execute(scene, context);

    // 根据模式选择专用Pass
    auto& activePasses = (m_currentMode == RenderMode::Forward) ? m_forwardPasses : m_deferredPasses;
    for (auto& pass : activePasses)
        pass->Execute(scene, context);

    for(auto& pass : m_postPasses) 
        pass->Execute(scene, context);
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
    // HDR
    context.GenHDR(width, height);
    // 后处理,MSAA
    context.GenForwad(width, height);
}

void ForwardPass::Execute(Scene &scene, RenderContext &context)
{
    std::shared_ptr<FrameBuffer> fbo;
    if(context.GetHDR())
        fbo = context.GetFBO(FBOType::HDR);
    else
        fbo = context.GetFBO(FBOType::Forward);
    fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto camera = scene.GetCamera();
    auto mainlight = scene.GetMainLight();
    for(auto entity : scene.GetEntities())
    {
        if(auto render = entity->GetComponent<RenderComponent>())
        {
            render->Render(*camera, mainlight, context.GetBloom());
        }
    }
    for(auto light: scene.GetLights())
    {
        if(auto render = light->GetComponent<RenderComponent>())
        {
            render->RenderLight(light, context.GetBloom());
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto mainlight = scene.GetMainLight();
    auto camera = scene.GetCamera();
    glm::mat4 vp = glm::mat4(1.0f);
    if(mainlight)
    {
        auto lc = mainlight->GetComponent<CameraComponent>();
        vp = lc->projection * std::get<glm::mat4>(lc->view);
    }
    for(auto entity : scene.GetEntities())
    {
        if(auto render = entity->GetComponent<RenderComponent>())
            render->RendergBuffer(*camera, vp, mainlight);
    }
    fbo->unbind();
}

void LightProcessPass::Execute(Scene &scene, RenderContext &context)
{
    auto gBuffer = context.GetFBO(FBOType::gBUFFER);
    gBuffer->bindTexture();

    std::shared_ptr<FrameBuffer> fbo;
    if(context.GetHDR())
        fbo = context.GetFBO(FBOType::HDR);
    else
        fbo = context.GetFBO(FBOType::Forward);
    fbo->bind();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = Import::GetShader("deffer");
    if(shader)
    {
        shader->use();
        // TODO:修改为从光源列表中获得
        auto light = scene.GetMainLight();
        shader->setVec3("lightPos", scene.GetCamera()->getView() * glm::vec4(light->GetComponent<TransformComponent>()->position, 1.0f));
        shader->setVec3("lightColor", light->GetComponent<LightComponent>()->color);

        shader->setBool("use_ssao", context.GetSSAO());
        shader->setFloat("bloom", context.GetBloom());
    
        RenderQuad::DrawwithShader(*shader);
    }
    else
        ERROR("deffer rendering light process fail!");

    // 前向渲染光源（也可用于混合等，以后单独拆分为一个pass）
    gBuffer->bindRead();
    fbo->bindDraw();
    gBuffer->blitDepth();
    fbo->bind();
    for(auto light: scene.GetLights())
    {
        if(auto render = light->GetComponent<RenderComponent>())
        {
            render->RenderLight(light, context.GetBloom());
        }
    }
    fbo->unbind();
}

void PostProcessPass::Execute(Scene &scene, RenderContext &context)
{
    std::shared_ptr<FrameBuffer> fbo;
    if(context.GetHDR())
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
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                blurShader->setInt("horizontal", horizontal);
                RenderQuad::DrawwithShader(*blurShader);
                if(horizontal)
                    blurh->bindTexture();
                else
                    blurv->bindTexture();
                
                horizontal = !horizontal;
            }
            ImGuiRenderer::imguiF->bind();
    
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
            fbo->bindSingleTexture(0);
            if(!horizontal)
                blurh->bindTexture(1);
            else
                blurv->bindTexture(1);
    
            auto hdrShader = Import::GetShader("HDR");
            if(hdrShader)
            {
                hdrShader->use();
                hdrShader->setFloat("exposure", context.GetExposure());
                RenderQuad::DrawwithShader(*hdrShader);
            }
            else
                ERROR("postprocess hdr fail!");
        }
        else
            ERROR("postprocess blur fail!");
    }
    else
    {
        fbo = context.GetFBO(FBOType::Forward);
        fbo->bindTexture();
        ImGuiRenderer::imguiF->bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto postShader = Import::GetShader("PostProcess");
        if(postShader)
            RenderQuad::DrawwithShader(*postShader);
        else
            ERROR("postprocess fail");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

SSAOPass::SSAOPass(RenderContext& context, int width, int height)
{
    // Sample kernel
    std::random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
    std::vector<glm::vec3> ssaoKernel;
    for (GLuint i = 0; i < 64; ++i)
    {
        glm::vec3 sample(randomFloats(gen) * 2.0 - 1.0, randomFloats(gen) * 2.0 - 1.0, randomFloats(gen));
        sample = glm::normalize(sample);
        sample *= randomFloats(gen);
        GLfloat scale = GLfloat(i) / 64.0;

        // Scale samples s.t. they're more aligned to center of kernel
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    auto shader = Import::GetShader("ssao");
    if(shader)
    {
        shader->use();
        for (GLuint i = 0; i < 64; ++i)
            shader->setVec3("samples[" + std::to_string(i) + "]", ssaoKernel[i]);
        shader->setVec2("screenSize", width, height);
    }
    else
        ERROR("ssao initialization fail!");

    // Noise texture
    std::vector<glm::vec3> ssaoNoise;
    for (GLuint i = 0; i < 16; i++)
    {
        glm::vec3 noise(randomFloats(gen) * 2.0 - 1.0, randomFloats(gen) * 2.0 - 1.0, 0.0f); // rotate around z-axis (in tangent space)
        ssaoNoise.push_back(noise);
    }

    context.GenSSAO(width, height, (void*)&ssaoNoise[0]);
}

void SSAOPass::Execute(Scene &scene, RenderContext &context)
{
    if(context.GetSSAO())
    {
        // 生成SSAO
        auto gBuffer = context.GetFBO(FBOType::gBUFFER);
        gBuffer->bindTexture();
    
        auto ssao = context.GetFBO(FBOType::SSAO);
        ssao->bind();
    
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        auto shader = Import::GetShader("ssao");
        if(shader)
        {
            shader->use();
            shader->setMat4("projection", scene.GetCamera()->getProjection());
            RenderQuad::DrawwithShader(*shader);
        }
        else
            ERROR("ssao fail!");
        
        // 模糊SSAO
        ssao->bindTexture();
        auto ssaoblur = context.GetFBO(FBOType::SSAOblur);
        ssaoblur->bind();
    
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
        shader = Import::GetShader("ssaoblur");
        if(shader)
        {
            shader->use();
            RenderQuad::DrawwithShader(*shader);
        }
        else
            ERROR("ssaoblur fail!");
        
        ssaoblur->unbind();
        ssaoblur->bindTexture(4);
    }
}