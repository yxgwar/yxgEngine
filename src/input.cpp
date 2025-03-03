#include "input.h"
#include <iostream>

Input::MouseState Input::currentMouseState;

void Input::OnUpdate()
{
    memcpy(Input::previousFrameKeys, Input::currentFrameKeys, sizeof(Input::currentFrameKeys));

    // 保存上一帧光标位置，用于计算下一帧的delta
    previousMouseX = currentMouseState.x;
    previousMouseY = currentMouseState.y;
    // 重置当前帧的delta和滚轮偏移
    currentMouseState.deltaX = 0.0;
    currentMouseState.deltaY = 0.0;
    // currentMouseState.scrollOffset = 0.0;
}

void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        currentFrameKeys[key] = true;
    }
    else if (action == GLFW_RELEASE)
    {
        currentFrameKeys[key] = false;
    }
}

void Input::CursorPosCallback(GLFWwindow *window, double xpos, double ypos)
{
    currentMouseState.deltaX = xpos - previousMouseX;
    currentMouseState.deltaY = ypos - previousMouseY;
    currentMouseState.x = xpos;
    currentMouseState.y = ypos;
}

void Input::MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        // currentMouseState.leftButtonDown = (action == GLFW_PRESS);
    }
}

void Input::ScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    // currentMouseState.scrollOffset = yoffset; // 通常只关注垂直滚动
}
