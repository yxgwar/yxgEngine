#pragma once

#include "renderer/camera.h"
#include "object/entity.h"
#include "renderer/renderpipeline.h"

class Scene
{
public:
    ~Scene();

    void Init(int width, int height);

    void OnLogicUpdate(float deltaTime, Window& window);
    void OnRenderUpdate(float deltaTime, RenderPipeline& pipeline);

    inline Entity* GetMainLight() const {return mainLight;}
    inline std::vector<Entity*>& GetEntities() {return m_entities;}
    inline std::vector<Entity*>& GetLights() {return m_lights;}
    inline Camera* GetCamera() const {return m_camera.get();}
private:
    std::unique_ptr<Camera> m_camera;
    std::vector<Entity*> m_lights;
    Entity* mainLight;
    std::vector<Entity*> m_entities;
    int id = 0;
};