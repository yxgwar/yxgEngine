#include <iostream>
#include <cmath>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "camera.h"
#include "model.h"
#include "framebuffer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

int ScreenWidth = 1920;
int ScreenHeight = 1080;

Camera camera((float)ScreenWidth, (float)ScreenHeight);

float deltaTime = 0.0f;
float lastTime = 0.0f;

bool isFocus = true;
bool isPressed = false;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int viewportWidth = ScreenWidth;
int viewportHeight = ScreenHeight;
int offsetX = 0;
int offsetY = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    float targetAspect = (float)ScreenWidth / ScreenHeight;
    float windowAspect = (float)width / height;

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    glViewport(0, 0, ScreenWidth, ScreenHeight);

    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_STENCIL_TEST);
    // glStencilFunc(GL_EQUAL, 1, 0xFF);
    // glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glEnable(GL_CULL_FACE);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    Shader modelShader("../assets/shaders/model.vs", "../assets/shaders/model.fs");
    Shader screenShader("../assets/shaders/frameScreen.vs", "../assets/shaders/frameScreen.fs");
    // Shader stencilShader("../assets/shaders/stencil.vs", "../assets/shaders/stencil.fs");
    Model loadmodel("../assets/models/backpack/backpack.obj");

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };

    VertexArray VAO;
    VertexBuffer VBO(quadVertices, sizeof(quadVertices));
    std::vector<VertexAttribute> attribute = {
        {0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0},
        {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))}
    };

    VAO.AddVBO(VBO, attribute);

    glfwSetWindowUserPointer(window, &modelShader);
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

    FrameBuffer FBO;
    FBO.attachColor(ScreenWidth, ScreenHeight);
    FBO.attachDepthStencil(ScreenWidth, ScreenHeight);

    screenShader.use();
    screenShader.setInt("screenTexture", 0);

    glm::vec3 pointLightPositions = glm::vec3( 0.7f,  0.2f,  2.0f);

    while(!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::string title = "FPS:" + std::to_string(1.0f / deltaTime);
        glfwSetWindowTitle(window, title.c_str());

        processInput(window);

        FBO.bind();
        glViewport(0, 0, ScreenWidth, ScreenHeight);
        glEnable(GL_DEPTH_TEST);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        modelShader.use();

        modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        modelShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        modelShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
        // point light 1
        modelShader.setVec3("pointLights.position", pointLightPositions);
        modelShader.setVec3("pointLights.ambient", 0.05f, 0.05f, 0.05f);
        modelShader.setVec3("pointLights.diffuse", 0.8f, 0.8f, 0.8f);
        modelShader.setVec3("pointLights.specular", 1.0f, 1.0f, 1.0f);
        modelShader.setFloat("pointLights.constant", 1.0f);
        modelShader.setFloat("pointLights.linear", 0.09f);
        modelShader.setFloat("pointLights.quadratic", 0.032f);

        modelShader.setVec3("viewPos", camera.getPosition());
        modelShader.setMat4("projection", camera.getProjection());
        modelShader.setMat4("view", camera.getView());
        modelShader.setMat4("model", loadmodel.getModel());
        modelShader.setMat4("NormalM", loadmodel.getNormalM());

        // glStencilFunc(GL_ALWAYS, 1, 0xFF);
        // glStencilMask(0xFF);

        loadmodel.Draw(modelShader);

        FBO.unbind();
        glViewport(offsetX, offsetY, viewportWidth, viewportHeight);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
        glClear(GL_COLOR_BUFFER_BIT);
        screenShader.use();
        FBO.bindTexture();
        VAO.Draw();
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        // glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        // glStencilMask(0x00);
        // glDisable(GL_DEPTH_TEST);

        // stencilShader.use();
        // stencilShader.setMat4("projection", camera.getProjection());
        // stencilShader.setMat4("view", camera.getView());
        // stencilShader.setMat4("model", loadmodel.getModel());
        // loadmodel.Draw(stencilShader);

        // glStencilMask(0xFF);
        // glStencilFunc(GL_ALWAYS, 0, 0xFF);
        // glEnable(GL_DEPTH_TEST);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

