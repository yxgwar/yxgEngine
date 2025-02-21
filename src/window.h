#pragma once

#include "log.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "renderer/camera.h"

struct WindowData
{
    Camera *camera;
};

class Window
{
public:
    Window(int width, int height);
    ~Window();

    void SetCallback(WindowData* windowData);
    void ProcessInput(Camera& camera, float deltaTime);
    void OnUpdate();

    inline bool OpenWindow() {return !glfwWindowShouldClose(m_window);}
    inline void SetTitle(std::string& title) {glfwSetWindowTitle(m_window, title.c_str());}
    inline float GetTime() {return glfwGetTime();}
    inline GLFWwindow* GetWindow() const {return m_window;}
private:
    float m_aspect;
    bool m_Focus;
    GLFWwindow* m_window;
};