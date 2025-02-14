#include <iostream>
#include <cmath>

#include "window.h"
#include "renderer/camera.h"
#include "object/model.h"
#include "renderer/uniformbuffer.h"
#include "renderer/renderer.h"

#include "custom/BlinnPhong.h"
#include "custom/Reflect.h"
#include "custom/Refract.h"
#include "custom/Explore.h"
#include "custom/Test.h"
#include "custom/NormalVisual.h"

void renderScene(Shader& shader)
{
    // floor
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(25.0f, 25.0f, 1.0f));
    shader.setMat4("model", model);
    RenderQuad::DrawwithShader(shader);
    // cubes
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    RenderCube::DrawwithShader(shader);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(0.5f));
    shader.setMat4("model", model);
    RenderCube::DrawwithShader(shader);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
    model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
    model = glm::scale(model, glm::vec3(0.25f));
    shader.setMat4("model", model);
    RenderCube::DrawwithShader(shader);
}

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
    RenderQuad::Init();
    RenderCube::Init();
    std::vector<std::string> faces
    {
        "../assets/images/skybox/right.jpg",
        "../assets/images/skybox/left.jpg",
        "../assets/images/skybox/top.jpg",
        "../assets/images/skybox/bottom.jpg",
        "../assets/images/skybox/front.jpg",
        "../assets/images/skybox/back.jpg"
    };
    // Renderer::InitSkybox(faces);

    // std::shared_ptr<Model> model = std::make_shared<Model>("../assets/models/backpack/backpack.obj");

    // std::vector<std::unique_ptr<ModelTest>> models;
    // models.emplace_back(std::make_unique<BlinnPhong>(model));
    // models.emplace_back(std::make_unique<Reflect>(model));
    // models.emplace_back(std::make_unique<Refract>(model));
    // models.emplace_back(std::make_unique<Explore>(model));
    // models.emplace_back(std::make_unique<Test>(model));
    // models.emplace_back(std::make_unique<NormalVisual>(model));

    UniformBuffer UBO(2 * sizeof(glm::mat4));
    UBO.bind(0, 2 * sizeof(glm::mat4));
    UBO.setData(0, sizeof(glm::mat4), glm::value_ptr(camera.getProjection()));

    Shader simpleDepthShader("../assets/shaders/shadow/simpleDepthShader.vs", "../assets/shaders/shadow/simpleDepthShader.fs");
    Shader debugShader("../assets/shaders/shadow/debug.vs", "../assets/shaders/shadow/debug.fs");
    debugShader.use();
    debugShader.setInt("depthMap", 0);

    Shader testShader("../assets/shaders/test.vs", "../assets/shaders/test.fs");
    testShader.use();
    testShader.setInt("texture_diffuse1", 0);

    Texture wood("../assets/images/wood.png");

    glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    float near_plane = 1.0f, far_plane = 7.5f;
    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
    // render scene from light's point of view
    simpleDepthShader.use();
    simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    while(window.OpenWindow())
    {
        float currentTime = window.GetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::string title = "FPS:" + std::to_string(1.0f / deltaTime);
        window.SetTitle(title);
        window.ProcessInput(camera, deltaTime);

        UBO.setData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getView()));

        Renderer::StartDrawDepthMap();
        // for(auto& m: models)
        // {
        //     m->StartDrawwithShader(camera, simpleDepthShader);
        // }
        wood.bind(0);
        renderScene(simpleDepthShader);
        Renderer::EndDrawDepthMap();

        Renderer::StartRender();
        // for(auto& m: models)
        // {
        //     // m->StartDraw(camera);
        //     m->StartDrawwithShader(camera, debugShader);
        // }
        
        // Renderer::DrawSkybox();
        debugShader.use();
        debugShader.setFloat("near_plane", near_plane);
        debugShader.setFloat("far_plane", far_plane);
        RenderQuad::DrawwithShader(debugShader);
        Renderer::EndRender();

        window.OnUpdate();
    }
    return 0;
}