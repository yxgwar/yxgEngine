#pragma once

#include "custom/PointLight.h"
#include "renderer/camera.h"
#include "object/entity.h"
#include "import/import.h"

class Scene
{
public:
    Scene();
    ~Scene();

    void Init(int width, int height);

    void OnUpdate();
private:
    // 测试用
    void loadAllShaders();
    void loadAllTextures();
    void loadAllMaterials();
    void loadAllModels();
private:
    std::unique_ptr<Camera> m_camera;
    PointLight m_light;
    std::vector<Entity*> m_entities;
    int id = 0;
};