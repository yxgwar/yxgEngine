#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <string>

class Window
{
public:
    Window(int width, int height);
    ~Window();

    void SetCallback();
    void ProcessInput();
    void OnUpdate();

    inline void Focus() {glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);}
    inline void UnFocus() {glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);}

    inline bool OpenWindow() {return !glfwWindowShouldClose(m_window);}
    inline void SetTitle(std::string& title) {glfwSetWindowTitle(m_window, title.c_str());}
    inline float GetTime() {return glfwGetTime();}
    inline GLFWwindow* GetWindow() const {return m_window;}

    inline int GetWidth() const {return m_width;}
    inline int GetHeight() const {return m_height;}
private:
    int m_width, m_height;
    float m_aspect;
    bool m_Focus;
    GLFWwindow* m_window;
};