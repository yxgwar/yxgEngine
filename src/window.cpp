#include "window.h"
#include "input.h"
#include "log.h"

Window::Window(int width, int height)
    :m_aspect(float(width) / height), m_width(width), m_height(height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_window = glfwCreateWindow(width, height, "LearnOpenGL", nullptr, nullptr);
    if (m_window == nullptr)
    {
        ERROR("Failed to create GLFW window");
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ERROR("Failed to initialize GLAD");
        throw std::runtime_error("Failed to initialize GLAD");
    }
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::SetCallback()
{
    glfwSetKeyCallback(m_window, Input::KeyCallback);
    glfwSetCursorPosCallback(m_window, Input::CursorPosCallback);
}

void Window::ProcessInput()
{
    glfwPollEvents();
}

void Window::OnUpdate()
{
    glfwSwapBuffers(m_window);
}