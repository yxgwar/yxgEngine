#pragma once

#include "window.h"
#include "renderer/RenderQuad.h"
#include "renderer/ImGuiRenderer.h"
#include "renderer/rendercontext.h"
#include "renderer/renderpipeline.h"
#include "scene.h"

class Application
{
public:
    Application(int width = 1920, int height = 1080);
    ~Application() = default;

    void Run();
private:
    int m_width, m_height;
    Window m_window;
    Scene m_scene;
    RenderPipeline m_renderPipeline;

    float m_lastTime = 0.0f;
};