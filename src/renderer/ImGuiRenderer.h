#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"
#include "renderer.h"

class ImGuiRenderer
{
public:
    static void Init(Window& window);
    static void Begin();
    static void End();
    static void Destroy();
    //imgui
    inline static std::unique_ptr<FrameBuffer> imguiF = nullptr;
private:
};