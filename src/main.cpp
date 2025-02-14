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

    std::shared_ptr<Model> model = std::make_shared<Model>("../assets/models/backpack/backpack.obj");

    std::vector<std::unique_ptr<ModelTest>> models;
    models.emplace_back(std::make_unique<BlinnPhong>(model));
    models.emplace_back(std::make_unique<Reflect>(model));
    models.emplace_back(std::make_unique<Refract>(model));
    models.emplace_back(std::make_unique<Explore>(model));
    models.emplace_back(std::make_unique<Test>(model));
    models.emplace_back(std::make_unique<NormalVisual>(model));

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

        UBO.setData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getView()));

        // Renderer::StartDrawDepthMap();
        // for(auto& object: objects)
        // {
        //     object->StartDrawwithTempShader(camera, simpleDepthShader);
        // }
        // Renderer::EndDrawDepthMap();

        Renderer::StartRender();
        for(auto& m: models)
        {
            m->StartDraw(camera);
        }
        Renderer::DrawSkybox();
        Renderer::EndRender();

        window.OnUpdate();
    }
    return 0;
}

