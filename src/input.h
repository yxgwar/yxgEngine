#pragma once

#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include <unordered_map>
#include <cstring>

class Input
{
public:
    //---------------- 鼠标状态 -----------------
    struct MouseState
    {
        float x = 0.0;       // 当前帧光标X
        float y = 0.0;       // 当前帧光标Y
        float deltaX = 0.0;  // 当前帧X偏移量
        float deltaY = 0.0;  // 当前帧Y偏移量
        // bool leftButtonDown = false; // 左键是否按下
        // double scrollOffset = 0.0;    // 滚轮偏移
    };

    inline static bool GetKey(int key) {return currentFrameKeys[key];}
    inline static bool GetKeyDown(int key) { return currentFrameKeys[key] && !previousFrameKeys[key];}

    // 获取当前帧鼠标状态（只读）
    inline static const MouseState& GetMouseState() { return currentMouseState; }

    static void OnUpdate();
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void CursorPosCallback(GLFWwindow* window, double xposIn, double yposIn);
    static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
private:
    static constexpr int MAX_KEYS = 512;
    inline static bool currentFrameKeys[MAX_KEYS] = {false};
    inline static bool previousFrameKeys[MAX_KEYS] = {false};

    static MouseState currentMouseState;
    inline static float previousMouseX = 0.0f;
    inline static float previousMouseY = 0.0f;

    Input() = delete;
};