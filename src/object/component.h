#pragma once

#include "mesh.h"
#include "material.h"
#include "import/import.h"
#include "renderer/camera.h"

class Entity;

class Component
{
public:
    Entity* owner = nullptr;
    virtual ~Component() = default;
    virtual void Update(float deltaTime) {} // 可选组件更新逻辑
};

// 变换组件
class TransformComponent: public Component
{
public:
    glm::vec3 position = glm::vec3(0.0f);
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f);

    void SetRotation(float radius, glm::vec3 direction);

    glm::mat4 GetTransformMatrix() const;
};

struct MeshSlot
{
    std::shared_ptr<Mesh> mesh;    // 几何数据
    int materialIndex;     // 指向材质数组的索引
    // TransformComponent localTransform; // 局部坐标系

    MeshSlot(std::shared_ptr<Mesh> m, int i)
        :mesh(m), materialIndex(i) {}
};

// 渲染组件
class RenderComponent: public Component
{
public:
    std::vector<std::shared_ptr<MeshSlot>> meshes;
    std::vector<std::shared_ptr<Material>> materials;

    void LoadModel(std::string path);

    void Render(Camera& camera) const;
};