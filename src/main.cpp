#include <iostream>
#include <cmath>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "shader.h"
#include "texture.h"
#include "vertexbuffer.h"
#include "indexbuffer.h"
#include "vertexarray.h"
#include "camera.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int ScreenWidth = 800;
int ScreenHeight = 600;

Camera camera((float)ScreenWidth, (float)ScreenHeight);

float deltaTime = 0.0f;
float lastTime = 0.0f;

bool isFocus = true;
bool isPressed = false;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float targetAspect = (float)ScreenWidth / ScreenHeight;
    float windowAspect = (float)width / height;

    int viewportWidth, viewportHeight, offsetX, offsetY;
    if(windowAspect > targetAspect)
    {
        viewportHeight = height;
        viewportWidth = (int)(height * targetAspect);
        offsetX = (width - viewportWidth) / 2;
        offsetY = 0;
    }
    else
    {
        viewportWidth = width;
        viewportHeight = (int)(width / targetAspect);
        offsetX = 0;
        offsetY = (height - viewportHeight) / 2;
    }
    glViewport(offsetX, offsetY, viewportWidth, viewportHeight);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS && !isPressed)
    {
        if(isFocus)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            isFocus = false;
            camera.FreeCamera();
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            isFocus = true;
        }
        isPressed = true;
    }
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        isPressed = false;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.KeyboardControl(CameraKeyCode::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.KeyboardControl(CameraKeyCode::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.KeyboardControl(CameraKeyCode::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.KeyboardControl(CameraKeyCode::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.KeyboardControl(CameraKeyCode::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.KeyboardControl(CameraKeyCode::DOWN, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if(isFocus)
    {
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);
        camera.MouseControl(xpos, ypos);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(isFocus)
        camera.ScrollControl(yoffset);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(ScreenWidth, ScreenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    glViewport(0, 0, 800, 600);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    VertexBuffer vb(vertices, sizeof(vertices));
    // IndexBuffer ib(indices, sizeof(indices));

    VertexArray va;
    std::vector<VertexAttribute> attributes = {
        {0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0},
        {1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))}
    };
    va.AddVBO(vb, attributes);

    VertexArray lightVAO;
    attributes = {
        {0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0}
    };
    lightVAO.AddVBO(vb, attributes);

    Shader boxShader("../assets/shaders/box.vs", "../assets/shaders/box.fs");
    Shader lightShader("../assets/shaders/light.vs", "../assets/shaders/light.fs");

    glfwSetWindowUserPointer(window, &boxShader);

    // 设置键盘回调
    glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_R && action == GLFW_PRESS) {
            Shader* shader = static_cast<Shader*>(glfwGetWindowUserPointer(window));
            if (shader->reload()) {
                std::cout << "Shader hot reload success" << std::endl;
            } else {
                std::cerr << "Shader hot reload fail" << std::endl;
            }
        }
    });

    // Texture texture1("../assets/images/container.jpg");
    // Texture texture2("../assets/images/awesomeface.png");

    // ourShader.use();
    // ourShader.setInt("texture1", 0);
    // ourShader.setInt("texture2", 1);

    while(!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::string title = "FPS:" + std::to_string(1.0f / deltaTime);
        glfwSetWindowTitle(window, title.c_str());

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // texture1.bind(0);
        // texture2.bind(1);
        boxShader.use();
        boxShader.setVec3("material.ambient", 0.25f, 0.20725f, 0.20725f);
        boxShader.setVec3("material.diffuse", 1.0f, 0.829f, 0.829f);
        boxShader.setVec3("material.specular", 0.296648f, 0.296648f, 0.296648f);
        boxShader.setFloat("material.shininess", 0.088f * 128);
        
        boxShader.setVec3("light.ambient",  1.0f, 1.0f, 1.0f);
        boxShader.setVec3("light.diffuse",  1.0f, 1.0f, 1.0f);
        boxShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f); 
        boxShader.setVec3("light.position", lightPos);
        boxShader.setVec3("viewPos", camera.getPosition());

        boxShader.setMat4("projection", camera.getProjection());
        boxShader.setMat4("view", camera.getView());

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 NormalM = glm::transpose(glm::inverse(model));
        boxShader.setMat4("model", model);
        boxShader.setMat4("NormalM", NormalM);

        va.Draw();

        lightShader.use();
        lightShader.setMat4("projection", camera.getProjection());
        lightShader.setMat4("view", camera.getView());
        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f));
        lightShader.setMat4("model", model);

        lightVAO.Draw();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

