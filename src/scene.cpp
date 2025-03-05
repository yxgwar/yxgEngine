#include "scene.h"

Scene::Scene()
{
    
}

Scene::~Scene()
{
    for(auto e: m_entities)
        delete e;
}

void Scene::Init(int width, int height)
{
    m_camera = std::make_unique<Camera>(width, height);

    // std::vector<std::string> faces
    // {
    //     "../assets/images/skybox/right.jpg",
    //     "../assets/images/skybox/left.jpg",
    //     "../assets/images/skybox/top.jpg",
    //     "../assets/images/skybox/bottom.jpg",
    //     "../assets/images/skybox/front.jpg",
    //     "../assets/images/skybox/back.jpg"
    // };
    // Renderer::InitSkybox(faces);

    // 加载场景
    Entity* entt = new Entity(id++, "raye");
    auto tc = entt->AddComponent<TransformComponent>();
    tc->position = glm::vec3(-2.0f, -0.5f, 0.0f);
    tc->scale = glm::vec3(0.2f);
    auto rc = entt->AddComponent<RenderComponent>();
    rc->LoadModel("../assets/models/raye/raye.pmx");
    m_entities.emplace_back(entt);

    entt = new Entity(id++, "roze");
    tc = entt->AddComponent<TransformComponent>();
    tc->position = glm::vec3(2.0f, -0.5f, 0.0f);
    tc->scale = glm::vec3(0.2f);
    rc = entt->AddComponent<RenderComponent>();
    rc->LoadModel("../assets/models/roze/roze.pmx");
    m_entities.emplace_back(entt);

    entt = new Entity(id++, "wood");
    tc = entt->AddComponent<TransformComponent>();
    tc->position = glm::vec3(0.0f, -0.5f, 0.0f);
    tc->SetRotation(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    tc->scale = glm::vec3(25.0f, 25.0f, 1.0f);
    rc = entt->AddComponent<RenderComponent>();
    rc->meshes = Import::MeshPool["quad"];
    rc->materials.emplace_back(Import::MaterialPool["default"]);
    std::string path = "../assets/images/wood.png";
    rc->materials[0]->AddTexture(Import::LoadTexture(path), "texture_diffuse", path);
    m_entities.emplace_back(entt);

    entt = new Entity(id++, "light");
    tc = entt->AddComponent<TransformComponent>();
    tc->position = glm::vec3(0.0f, 5.0f, 5.0f);
    tc->scale = glm::vec3(0.1f);
    rc = entt->AddComponent<RenderComponent>();
    rc->meshes = Import::MeshPool["quad"];
    rc->materials.emplace_back(std::make_shared<Material>(std::make_shared<Shader>("../assets/shaders/default/default.vs", "../assets/shaders/default/default.fs")));
    rc->materials[0]->AddTexture(Import::TexturePool["white"], "texture_diffuse");
    auto lc = entt->AddComponent<LightComponent>();
    lc->type = LightComponent::Type::Directional;
    lc->direction = -tc->position;
    tc->rotation = glm::quat(glm::vec3(0.0f, 0.0f, 1.0f), lc->direction);
    lc->EnableShadow();
    mainLight = entt;
    m_entities.emplace_back(entt);
}

void Scene::OnLogicUpdate(float deltaTime, Window& window)
{
    m_camera->OnUpdate(deltaTime, window);
}

void Scene::OnRenderUpdate(float deltaTime, RenderPipeline& pipeline)
{
    pipeline.UpdateGlobalUBO(m_camera.get(), RenderContext::GetInstance());
    pipeline.Execute(*this, RenderContext::GetInstance());
}
