#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"

class ImGuiRenderer
{
public:
    static void Init(Window& window);
    static void Create();
    static void Draw();
};