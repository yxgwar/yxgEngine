#include "application.h"
#include "imguiRenderer/ImGuiRenderer.h"
#include "renderer/rendercontext.h"
#include "renderer/RenderQuad.h"
#include "import/import.h"
#include "input.h"

Application::Application(int width, int height)
    :m_width(width), m_height(height),
    m_window(width, height)
{
    m_window.SetCallback();

    RenderQuad::Init();

    Import::GenDefault();

    m_scene.Init(m_width, m_height);

    RenderContext& renderContext = RenderContext::GetInstance();
    bool hdr = true;
    m_renderPipeline.AddCommonPass(std::make_unique<PrePass>(renderContext));
    m_renderPipeline.AddCommonPass(std::make_unique<ShadowMapPass>(renderContext));
    m_renderPipeline.AddForwardPass(std::make_unique<ForwardPass>(renderContext, m_width, m_height));
    m_renderPipeline.AddDeferredPass(std::make_unique<GBufferPass>(renderContext, m_width, m_height));
    m_renderPipeline.AddDeferredPass(std::make_unique<SSAOPass>(renderContext, m_width, m_width));
    m_renderPipeline.AddDeferredPass(std::make_unique<LightProcessPass>());
    m_renderPipeline.AddPostProcessPass(std::make_unique<PostProcessPass>());
    
    ImGuiRenderer::Init(m_window, m_renderPipeline);
}

void Application::Run()
{
    while(m_window.OpenWindow())
    {
        float currentTime = m_window.GetTime();
        float deltaTime = currentTime - m_lastTime;
        m_lastTime = currentTime;

        m_window.ProcessInput();
        
        m_scene.OnLogicUpdate(deltaTime, m_window);
        m_scene.OnRenderUpdate(deltaTime, m_renderPipeline);

        ImGuiRenderer::OnUpdate();
        m_window.OnUpdate();

        Input::OnUpdate();
    }
    ImGuiRenderer::Destroy();
}
