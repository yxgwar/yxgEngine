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

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

int ScreenWidth = 1920;
int ScreenHeight = 1080;

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

    glViewport(0, 0, ScreenWidth, ScreenHeight);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f,  0.2f,  2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f,  2.0f, -12.0f),
        glm::vec3( 0.0f,  0.0f, -3.0f)
    };

    VertexBuffer vb(vertices, sizeof(vertices));
    // IndexBuffer ib(indices, sizeof(indices));

    VertexArray boxVAO;
    std::vector<VertexAttribute> attributes = {
        {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0},
        {1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))},
        {2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))}
    };
    boxVAO.AddVBO(vb, attributes);

    VertexArray lightVAO;
    attributes = {
        {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0}
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
    Texture diffuse("../assets/images/container2.png");
    Texture specular("../assets/images/container2_specular.png");
    Texture sky("../assets/images/sky.jpg");
    // Texture emission("../assets/images/matrix.jpg");

    while(!glfwWindowShouldClose(window))
    {
        float currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::string title = "FPS:" + std::to_string(1.0f / deltaTime);
        glfwSetWindowTitle(window, title.c_str());
        glfwGetWindowSize(window, &ScreenWidth, &ScreenHeight);

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        diffuse.bind(0);
        specular.bind(1);
        sky.bind(2);
        // emission.bind(2);

        boxShader.use();
        boxShader.setInt("material.diffuse", 0);
        boxShader.setInt("material.specular", 1);
        boxShader.setInt("lightTex", 2);
        boxShader.setFloat("material.shininess", 32.0f);
        
        boxShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
        boxShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
        boxShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
        boxShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
                // point light 1
        boxShader.setVec3("pointLights[0].position", pointLightPositions[0]);
        boxShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        boxShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
        boxShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        boxShader.setFloat("pointLights[0].constant", 1.0f);
        boxShader.setFloat("pointLights[0].linear", 0.09f);
        boxShader.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        boxShader.setVec3("pointLights[1].position", pointLightPositions[1]);
        boxShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        boxShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
        boxShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        boxShader.setFloat("pointLights[1].constant", 1.0f);
        boxShader.setFloat("pointLights[1].linear", 0.09f);
        boxShader.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        boxShader.setVec3("pointLights[2].position", pointLightPositions[2]);
        boxShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        boxShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
        boxShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        boxShader.setFloat("pointLights[2].constant", 1.0f);
        boxShader.setFloat("pointLights[2].linear", 0.09f);
        boxShader.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        boxShader.setVec3("pointLights[3].position", pointLightPositions[3]);
        boxShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        boxShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
        boxShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        boxShader.setFloat("pointLights[3].constant", 1.0f);
        boxShader.setFloat("pointLights[3].linear", 0.09f);
        boxShader.setFloat("pointLights[3].quadratic", 0.032f);
        // spotLight
        boxShader.setVec3("spotLight.position", camera.getPosition());
        boxShader.setVec3("spotLight.direction", camera.getFront());
        boxShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
        boxShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
        boxShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
        boxShader.setFloat("spotLight.constant", 1.0f);
        boxShader.setFloat("spotLight.linear", 0.09f);
        boxShader.setFloat("spotLight.quadratic", 0.032f);
        boxShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
        boxShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

        boxShader.setVec3("viewPos", camera.getPosition());

        boxShader.setMat4("projection", camera.getProjection());
        boxShader.setMat4("view", camera.getView());
        if(ScreenWidth >= ScreenHeight)
            boxShader.setVec2f("resolution", ScreenWidth, ScreenHeight);
        else
            boxShader.setVec2f("resolution", ScreenHeight, ScreenWidth);

        for(unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glm::mat4 NormalM = glm::transpose(glm::inverse(model));
            boxShader.setMat4("model", model);
            boxShader.setMat4("NormalM", NormalM);
            boxVAO.Draw();
        }

        lightShader.use();
        lightShader.setMat4("projection", camera.getProjection());
        lightShader.setMat4("view", camera.getView());
        for (unsigned int i = 0; i < 4; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.2f));
            lightShader.setMat4("model", model);
            lightVAO.Draw();
        }

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

