#include "window.h"

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
}

Window::~Window()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Window::SetCallback(WindowData *windowData)
{
    glfwSetWindowUserPointer(m_window, windowData);

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, int width, int height)
    {    
        glViewport(0, 0, width, height);
        // float windowAspect = (float)width / height;
        // if(windowAspect > m_aspect)
        // {
        //     viewportHeight = height;
        //     viewportWidth = (int)(height * targetAspect);
        //     offsetX = (width - viewportWidth) / 2;
        //     offsetY = 0;
        // }
        // else
        // {
        //     viewportWidth = width;
        //     viewportHeight = (int)(width / targetAspect);
        //     offsetX = 0;
        //     offsetY = (height - viewportHeight) / 2;
        // }
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xposIn, double yposIn)
    {
        WindowData* windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if(windowData->camera->Foucs)
        {
            float xpos = static_cast<float>(xposIn);
            float ypos = static_cast<float>(yposIn);
            windowData->camera->MouseControl(xpos, ypos);
        }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xoffset, double yoffset)
    {
        WindowData* windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if(windowData->camera->Foucs)
            windowData->camera->ScrollControl(yoffset);
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        WindowData* windowData = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        Camera* camera = windowData->camera;
        switch (action) {
            case GLFW_PRESS:
            {
                if(key == GLFW_KEY_ESCAPE)
                {
                    if(camera->Foucs)
                    {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                        camera->FreeCamera();
                    }
                    else
                    {
                        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                        camera->ActiveCamera();
                    }
                }
                break;
            }
            case GLFW_RELEASE:
            {
                break;
            }
            case GLFW_REPEAT:
            {
                break;
            }
        }
    });

    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

//暂时使用
void Window::ProcessInput(Camera &camera, float deltaTime)
{
    if(camera.Foucs)
    {
        if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
            camera.KeyboardControl(CameraKeyCode::FORWARD, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
            camera.KeyboardControl(CameraKeyCode::BACKWARD, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
            camera.KeyboardControl(CameraKeyCode::LEFT, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
            camera.KeyboardControl(CameraKeyCode::RIGHT, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
            camera.KeyboardControl(CameraKeyCode::UP, deltaTime);
        if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
            camera.KeyboardControl(CameraKeyCode::DOWN, deltaTime);
    }
}

void Window::OnUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

    // glfwSetWindowUserPointer(window, &modelShader);
    // // 设置键盘回调
    // glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
    //     if (key == GLFW_KEY_R && action == GLFW_PRESS) {
    //         Shader* shader = static_cast<Shader*>(glfwGetWindowUserPointer(window));
    //         if (shader->reload()) {
    //             std::cout << "Shader hot reload success" << std::endl;
    //         } else {
    //             std::cerr << "Shader hot reload fail" << std::endl;
    //         }
    //     }
    // });