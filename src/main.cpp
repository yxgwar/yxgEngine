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
#include "custom/BlinnPhongObject.h"
#include "custom/ReflectObject.h"
#include "custom/RefractObject.h"
#include "custom/ExploreObject.h"
#include "custom/TestObject.h"
#include "custom/NormalVisualObject.h"

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

    std::shared_ptr<Model> model = std::make_shared<Model>("../assets/models/backpack/backpack.obj");

    std::vector<std::unique_ptr<ModelTest>> objects;
    objects.emplace_back(std::make_unique<BlinnPhongObject>(model));
    objects.emplace_back(std::make_unique<ReflectObject>(model));
    objects.emplace_back(std::make_unique<RefractObject>(model));
    objects.emplace_back(std::make_unique<ExploreObject>(model));
    objects.emplace_back(std::make_unique<TestObject>(model));
    objects.emplace_back(std::make_unique<NormalVisualObject>(model));

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

        fbo.StartDrawOnFrameBuffer();
        Renderer::RendererStart();

        for(auto& object: objects)
        {
            object->StartDraw(camera);
        }

        skybox.StartDrawSkybox();

        fbo.StartDrawFrameBuffer();

        window.OnUpdate();
    }
    return 0;
}

