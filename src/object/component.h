#pragma once

#include "mesh.h"
#include "material.h"
#include "import/import.h"
#include "renderer/camera.h"
#include <variant>
#include <array>

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

    void Render(Camera& camera, Entity* light) const;
    void RenderDepth(glm::mat4& lightCamera) const;
};

// 摄像机
class CameraComponent: public Component
{
public:
    using lightMatrices = std::variant<glm::mat4, std::array<glm::mat4, 6>>;
    lightMatrices view;
    glm::mat4 projection;
};

// 光源(暂未添加更新功能)
class LightComponent: public Component
{
public:
    glm::vec3 color = glm::vec3(1.0f);
    float strength = 1.0f;
    bool castShadow = false;

    enum class Type { Point, Directional, Spot };
    Type type;

    glm::vec3 direction = glm::vec3(0.0f, 0.0f, -1.0f);
    float theta = 45.0f;

    void EnableShadow();
};