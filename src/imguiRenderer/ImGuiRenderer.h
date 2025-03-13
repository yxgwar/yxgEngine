#pragma once

#include "window.h"
#include "renderer/openGL/framebuffer.h"
#include "renderer/renderpipeline.h"
#include <memory>

class ImGuiRenderer
{
public:
    static void Init(Window& window, RenderPipeline& rp);
    static void OnUpdate();
    static void Destroy();
    inline static std::unique_ptr<FrameBuffer> imguiF = nullptr;
private:
    static void Begin();
    static void End();

    static void DrawGlobal();
private:
    inline static int m_width, m_height;
    inline static RenderPipeline* m_renderPipeline;
};