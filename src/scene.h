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

    void OnLogicUpdate(float deltaTime, Window& window);
    void OnRenderUpdate(float deltaTime, Window& window);
private:
    std::unique_ptr<Camera> m_camera;
    PointLight m_light;
    std::vector<Entity*> m_entities;
    int id = 0;
};