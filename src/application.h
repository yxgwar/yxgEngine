#pragma once

#include "window.h"
#include "renderer/renderer.h"
#include "renderer/ImGuiRenderer.h"
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

    float m_lastTime = 0.0f;
};