#include <iostream>
#include <cmath>

#include "window.h"
#include "renderer/camera.h"
#include "object/model.h"
#include "renderer/uniformbuffer.h"
#include "renderer/renderer.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "custom/FrameBufferObject.h"
#include "custom/SkyboxObject.h"

int main()
{
    int ScreenWidth = 1920;
    int ScreenHeight = 1080;

    Window window(ScreenWidth, ScreenHeight);
    Camera camera((float)ScreenWidth, (float)ScreenHeight);
    WindowData windowData;
    windowData.camera = &camera;
    window.SetCallback(&windowData);

    float deltaTime = 0.0f;
    float lastTime = 0.0f;

    Renderer::Init(ScreenWidth, ScreenHeight);

    FrameBufferObject fbo(ScreenWidth, ScreenHeight);
    SkyboxObject skybox;

    Shader modelShader("../assets/shaders/model.vs", "../assets/shaders/model.fs");
    Shader reflectShader("../assets/shaders/reflect.vs", "../assets/shaders/reflect.fs");
    Shader refractShader("../assets/shaders/reflect.vs", "../assets/shaders/refract.fs");
    Shader testShader("../assets/shaders/test.vs", "../assets/shaders/test.fs");
    Shader exploreShader("../assets/shaders/explore/explore.vs", "../assets/shaders/explore/explore.fs", "../assets/shaders/explore/explore.gs");
    Shader normalVisualShader("../assets/shaders/normalVisual/normalVisual.vs", "../assets/shaders/normalVisual/normalVisual.fs", "../assets/shaders/normalVisual/normalVisual.gs");
    Model loadmodel("../assets/models/backpack/backpack.obj");

    glm::vec3 pointLightPositions = glm::vec3( 0.7f,  0.2f,  2.0f);
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

    reflectShader.use();
    reflectShader.setInt("skybox", 0);

    refractShader.use();
    refractShader.setInt("skybox", 0);

    UniformBuffer UBO(2 * sizeof(glm::mat4));
    UBO.bind(0, 2 * sizeof(glm::mat4));
    UBO.setData(0, sizeof(glm::mat4), glm::value_ptr(camera.getProjection()));

    while(window.OpenWindow())
    {
        float currentTime = window.GetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::string title = "FPS:" + std::to_string(1.0f / deltaTime);
        window.SetTitle(title);
        window.ProcessInput(camera, deltaTime);

        Renderer::RendererStart();

        UBO.setData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getView()));

        fbo.StartDrawOnFrameBuffer();

        modelShader.use();
        loadmodel.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        modelShader.setVec3("viewPos", camera.getPosition());
        modelShader.setMat4("model", loadmodel.getModel());
        modelShader.setMat4("NormalM", loadmodel.getNormalM());
        loadmodel.Draw(modelShader);

        reflectShader.use();
        loadmodel.SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));
        reflectShader.setVec3("cameraPos", camera.getPosition());
        reflectShader.setMat4("model", loadmodel.getModel());
        reflectShader.setMat4("NormalM", loadmodel.getNormalM());
        loadmodel.Draw(reflectShader);

        refractShader.use();
        loadmodel.SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
        refractShader.setVec3("cameraPos", camera.getPosition());
        refractShader.setMat4("model", loadmodel.getModel());
        refractShader.setMat4("NormalM", loadmodel.getNormalM());
        loadmodel.Draw(refractShader);

        exploreShader.use();
        loadmodel.SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
        exploreShader.setMat4("model", loadmodel.getModel());
        exploreShader.setFloat("time", glfwGetTime());
        loadmodel.Draw(exploreShader);

        testShader.use();
        loadmodel.SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
        testShader.setMat4("model", loadmodel.getModel());
        loadmodel.Draw(testShader);

        normalVisualShader.use();
        normalVisualShader.setMat4("model", loadmodel.getModel());
        normalVisualShader.setMat4("NormalM", loadmodel.getNormalM());
        loadmodel.Draw(normalVisualShader);

        skybox.StartDrawSkybox();

        fbo.StartDrawFrameBuffer();

        window.OnUpdate();
    }
    return 0;
}

