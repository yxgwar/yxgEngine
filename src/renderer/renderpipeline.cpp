#include "renderpipeline.h"
#include "scene.h"
#include "RenderQuad.h"
#include "ImGuiRenderer.h"
#include "import/import.h"
#include <iostream>
#include <random>

float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

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
    auto camera = scene.GetCamera();
    glm::mat4 vp = glm::mat4(1.0f);
    if(light)
    {
        auto lc = light->GetComponent<CameraComponent>();
        vp = lc->projection * std::get<glm::mat4>(lc->view);
    }
    for(auto entity : scene.GetEntities())
    {
        if(auto render = entity->GetComponent<RenderComponent>())
            render->RendergBuffer(*camera, vp, light);
    }
    fbo->unbind();
}

void LightProcessPass::Execute(Scene &scene, RenderContext &context)
{
    auto gBuffer = context.GetFBO(FBOType::gBUFFER);
    gBuffer->bindTexture();

    auto ssao = context.GetFBO(FBOType::SSAOblur);
    ssao->bindTexture(4);

    ImGuiRenderer::imguiF->bind();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = Import::GetShader("deffer");
    if(shader)
    {
        shader->use();
        // TODO:修改为从光源列表中获得
        auto light = scene.GetMainLight();
        shader->setVec3("lightPos", scene.GetCamera()->getView() * glm::vec4(light->GetComponent<TransformComponent>()->position, 1.0f));
        shader->setVec3("lightColor", light->GetComponent<LightComponent>()->color);
    
        RenderQuad::DrawwithShader(*shader);
    }
    else
    {
        std::cout << "deffer shader get error!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
        std::cout << "ssao shader get error!" << std::endl;

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
    auto gBuffer = context.GetFBO(FBOType::gBUFFER);
    gBuffer->bindTexture();

    auto ssao = context.GetFBO(FBOType::SSAO);
    ssao->bind();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    auto shader = Import::GetShader("ssao");
    if(shader)
    {
        shader->use();
        shader->setMat4("projection", scene.GetCamera()->getProjection());
        RenderQuad::DrawwithShader(*shader);
    }
    else
        std::cout << "ssao shader get error!" << std::endl;
    
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
        std::cout << "ssao shader get error!" << std::endl;
}