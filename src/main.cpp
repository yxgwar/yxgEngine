#include <iostream>
#include <cmath>

#include "window.h"
#include "renderer/camera.h"
#include "object/model.h"
#include "renderer/renderer.h"

#include "custom/PointLight.h"

#include "custom/EmptyModel.h"

void renderScene(Shader& shader)
{
    // floor
    shader.use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(25.0f, 25.0f, 1.0f));
    shader.setMat4("model", model);
    shader.setMat3("NormalM", glm::transpose(glm::inverse(glm::mat3(model))));
    RenderQuad::DrawwithShader(shader);
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
    Renderer::InitSkybox(faces);

    // std::shared_ptr<Model> model = std::make_shared<Model>("../assets/models/backpack/backpack.obj");
    std::shared_ptr<Model> model1 = std::make_shared<Model>("../assets/models/raye/raye.pmx");
    EmptyModel raye(model1);
    raye.SetPosition(glm::vec3(-2.0f, -0.5f, 0.0f));
    // raye.SetRotation(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    raye.SetScale(glm::vec3(0.2f));
    std::shared_ptr<Model> model2 = std::make_shared<Model>("../assets/models/roze/roze.pmx");
    EmptyModel roze(model2);
    roze.SetPosition(glm::vec3(2.0f, -0.5f, 0.0f));
    // roze.SetRotation(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    roze.SetScale(glm::vec3(0.2f));

    // std::vector<std::unique_ptr<ModelTest>> models;
    // models.emplace_back(std::make_unique<BlinnPhong>(model));
    // models.emplace_back(std::make_unique<Reflect>(model));
    // models.emplace_back(std::make_unique<Refract>(model));
    // models.emplace_back(std::make_unique<Explore>(model));
    // models.emplace_back(std::make_unique<Test>(model));
    // models.emplace_back(std::make_unique<NormalVisual>(model));

    Renderer::SetCameraUBO(camera);

    // Shader simpleDepthShader("../assets/shaders/shadow/simpleDepthShader.vs", "../assets/shaders/shadow/simpleDepthShader.fs");
    Shader depthShader("../assets/shaders/PointLight/depth.vs", "../assets/shaders/PointLight/depth.fs", "../assets/shaders/PointLight/depth.gs");
    Shader debugShader("../assets/shaders/shadow/debug.vs", "../assets/shaders/shadow/debug.fs");
    debugShader.use();
    debugShader.setInt("depthMap", 0);

    PointLight light;
    light.SetPosition(glm::vec3(0.0f, 5.0f, 5.0f));
    light.SetScale(glm::vec3(0.1f));

    // Shader shadowMapShader("../assets/shaders/shadow/shadow.vs", "../assets/shaders/shadow/shadow.fs");
    Shader shadowMapShader("../assets/shaders/PointLight/shadow.vs", "../assets/shaders/PointLight/shadow.fs");
    shadowMapShader.use();
    shadowMapShader.setInt("texture_diffuse1", 0);
    shadowMapShader.setVec3("pointLightPos", light.GetPosition());
    shadowMapShader.setInt("pointShadowMap", 4);

    Shader testShader("../assets/shaders/test.vs", "../assets/shaders/test.fs");
    testShader.use();
    testShader.setInt("texture_diffuse1", 0);

    Texture wood("../assets/images/wood.png");

    // glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
    // glm::mat4 lightProjection, lightView;
    // glm::mat4 lightSpaceMatrix;
    // float near_plane = 1.0f, far_plane = 7.5f;
    // lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    // lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    // lightSpaceMatrix = lightProjection * lightView;
    // // render scene from light's point of view
    // simpleDepthShader.use();
    // simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

    while(window.OpenWindow())
    {
        float currentTime = window.GetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        std::string title = "FPS:" + std::to_string(1.0f / deltaTime);
        window.SetTitle(title);
        window.ProcessInput(camera, deltaTime);

        Renderer::UpdateCameraUBO(camera);

        light.StartDrawDepthMap(depthShader);
        raye.StartDrawwithShader(camera, depthShader);
        roze.StartDrawwithShader(camera, depthShader);
        light.StopDrawDepthMap();

        // Renderer::StartDrawDepthMap();
        // renderScene(simpleDepthShader);
        // raye.StartDrawwithShader(camera, simpleDepthShader);
        // roze.StartDrawwithShader(camera, simpleDepthShader);
        // Renderer::EndDrawDepthMap();

        Renderer::StartRender();
        shadowMapShader.use();
        shadowMapShader.setVec3("viewPos", camera.getPosition());
        shadowMapShader.setFloat("time", currentTime);

        light.BindDepthMap(4);

        // shadowMapShader.setVec3("lightPos", lightPos);
        // shadowMapShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
        wood.bind(0);
        renderScene(shadowMapShader);
        raye.StartDrawwithShader(camera, shadowMapShader);
        roze.StartDrawwithShader(camera, shadowMapShader);

        light.Draw();
        
        Renderer::DrawSkybox();
        // debugShader.use();
        // debugShader.setFloat("near_plane", near_plane);
        // debugShader.setFloat("far_plane", far_plane);
        // RenderQuad::DrawwithShader(debugShader);
        Renderer::EndRender();

        window.OnUpdate();
    }
    return 0;
}