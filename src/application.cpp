#include "application.h"

Application::Application(int width, int height)
    :m_width(width), m_height(height),
    m_window(width, height)
{
    m_window.SetCallback();

    Renderer::Init(m_width, m_height);

    ImGuiRenderer::Init(m_window);

    Import::GenDefaultMaterial();

    m_scene.Init(m_width, m_height);
}

void Application::Run()
{
    while(m_window.OpenWindow())
    {
        float currentTime = m_window.GetTime();
        float deltaTime = currentTime - m_lastTime;
        m_lastTime = currentTime;

        m_scene.OnUpdate();

        // ImGuiRenderer::Begin();
        // ImGuiRenderer::End();

        m_window.OnUpdate();
    }
    ImGuiRenderer::Destroy();
}
