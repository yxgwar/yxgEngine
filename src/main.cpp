#include <iostream>
#include <cmath>

#include "window.h"
#include "renderer/camera.h"
#include "object/model.h"
#include "renderer/renderer.h"

#include "custom/PointLight.h"

#include "custom/EmptyModel.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

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

    Renderer::SetCameraUBO(camera);

    // Shader simpleDepthShader("../assets/shaders/shadow/simpleDepthShader.vs", "../assets/shaders/shadow/simpleDepthShader.fs");
    Shader depthShader("../assets/shaders/PointLight/depth.vs", "../assets/shaders/PointLight/depth.fs", "../assets/shaders/PointLight/depth.gs");
    Shader debugShader("../assets/shaders/shadow/debug.vs", "../assets/shaders/shadow/debug.fs");
    debugShader.use();
    debugShader.setInt("depthMap", 0);

    PointLight light;
    glm::vec3 lightP(0.0f, 5.0f, 5.0f);
    light.SetPosition(lightP);
    light.SetScale(glm::vec3(0.1f));

    // Shader shadowMapShader("../assets/shaders/shadow/shadow.vs", "../assets/shaders/shadow/shadow.fs");
    Shader shadowMapShader("../assets/shaders/PointLight/shadow.vs", "../assets/shaders/PointLight/shadow.fs");
    shadowMapShader.use();
    shadowMapShader.setInt("texture_diffuse1", 0);
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

        // std::string title = "FPS:" + std::to_string(1.0f / deltaTime);
        // window.SetTitle(title);
        window.ProcessInput(camera, deltaTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        {
            ImGui::Begin("Hello, world!");
            ImGui::Text("This is some useful text.");
            ImGui::DragFloat("##X", &lightP.x, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::DragFloat("##Y", &lightP.y, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::DragFloat("##Z", &lightP.z, 0.1f, 0.0f, 0.0f, "%.2f");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        ImGui::Render();
        
        light.SetPosition(lightP);

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
        shadowMapShader.setVec3("pointLightPos", light.GetPosition());
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

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.OnUpdate();
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}