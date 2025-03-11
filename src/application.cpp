#include "application.h"
#include "renderer/ImGuiRenderer.h"
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

    ImGuiRenderer::Init(m_window);

    Import::GenDefault();

    m_scene.Init(m_width, m_height);

    RenderContext& renderContext = RenderContext::GetInstance();
    bool hdr = true;
    m_renderPipeline.AddPass(std::make_unique<PrePass>(renderContext));
    m_renderPipeline.AddPass(std::make_unique<ShadowMapPass>(renderContext));
    m_renderPipeline.AddPass(std::make_unique<ForwardPass>(renderContext, hdr, m_width, m_height));
    m_renderPipeline.AddPass(std::make_unique<PostProcessPass>(hdr));
    // m_renderPipeline.AddPass(std::make_unique<GBufferPass>(renderContext, m_width, m_height));
    // m_renderPipeline.AddPass(std::make_unique<LightProcessPass>());
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

        ImGuiRenderer::Begin();
        ImGuiRenderer::End();
        m_window.OnUpdate();

        Input::OnUpdate();
    }
    ImGuiRenderer::Destroy();
}
