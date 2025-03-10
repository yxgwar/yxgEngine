#include "component.h"
#include "entity.h"
#include "import/import.h"

void TransformComponent::SetRotation(float radius, glm::vec3 direction)
{
    rotation = glm::angleAxis(glm::radians(radius), glm::normalize(direction));
}

glm::mat4 TransformComponent::GetTransformMatrix() const
{
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
    matrix *= glm::mat4_cast(rotation);
    return glm::scale(matrix, scale);
}

void RenderComponent::LoadModel(std::string path)
{
    Import::LoadModel(path, meshes, materials);
}

void RenderComponent::Render(Camera &camera, Entity* light) const
{
    if (auto transform = owner->GetComponent<TransformComponent>())
    {
        glm::mat4 entityMatrix = transform->GetTransformMatrix();
        // 按材质索引分组MeshSlot
        std::unordered_map<int, std::vector<std::shared_ptr<MeshSlot>>> materialGroups;
        for (const auto& meshSlot : meshes)
        {
            int matIndex = meshSlot->materialIndex;
            materialGroups[matIndex].emplace_back(meshSlot);
        }

        auto rc = light->GetComponent<TransformComponent>();
        auto lc = light->GetComponent<LightComponent>();
        auto cc = light->GetComponent<CameraComponent>();
        
        // 遍历每个材质组
        for (const auto& [index, meshslots] : materialGroups)
        {
            const auto& material = materials[index];

            // 绑定材质并设置全局参数（如相机）
            material->SetMatrix4("lightSpaceMatrix", cc->projection * std::get<glm::mat4>(cc->view));
            material->SetVector3("viewPos", camera.getPosition());
            material->SetMatrix4("model", entityMatrix);
            material->SetMatrix3("NormalM", glm::transpose(glm::inverse(glm::mat3(entityMatrix))));
            material->SetVector3("lightPos", rc->position);
            material->SetVector3("lightColor", lc->color);
            material->Bind();

            // 绘制该材质下的所有MeshSlot
            for (const auto& meshslot : meshslots)
            {
                // 计算模型矩阵：Entity变换 * Mesh局部变换
                // glm::mat4 modelMatrix = entityMatrix * slot->localTransform.GetMatrix();

                // 设置实例参数（如模型矩阵）
                // material->SetMatrix4("model", modelMatrix);

                // 绘制Mesh
                meshslot->mesh->Draw();
            }
        }
    }
}

void RenderComponent::RenderDepth(glm::mat4 &lightCamera) const
{
    if (auto transform = owner->GetComponent<TransformComponent>())
    {
        glm::mat4 entityMatrix = transform->GetTransformMatrix();

        auto material = Import::MaterialPool["depthMap"];
        // 绑定材质并设置全局参数（如相机）
        material->SetMatrix4("lightSpaceMatrix", lightCamera);
        material->SetMatrix4("model", entityMatrix);
        material->Bind();

        // 绘制该材质下的所有MeshSlot
        for (const auto& meshslot : meshes)
        {
            meshslot->mesh->Draw();
        }
    }
}

void RenderComponent::RendergBuffer(glm::mat4 &lightCamera, Entity* light) const
{
    if (auto transform = owner->GetComponent<TransformComponent>())
    {
        glm::mat4 entityMatrix = transform->GetTransformMatrix();
        // 按材质索引分组MeshSlot
        std::unordered_map<int, std::vector<std::shared_ptr<MeshSlot>>> materialGroups;
        for (const auto& meshSlot : meshes)
        {
            int matIndex = meshSlot->materialIndex;
            materialGroups[matIndex].emplace_back(meshSlot);
        }
        
        auto rc = light->GetComponent<TransformComponent>();

        // 遍历每个材质组
        for (const auto& [index, meshslots] : materialGroups)
        {
            const auto& material = materials[index];

            material->SetMatrix4("model", entityMatrix);
            material->SetMatrix3("NormalM", glm::transpose(glm::inverse(glm::mat3(entityMatrix))));
            material->SetMatrix4("lightSpaceMatrix", lightCamera);
            material->SetVector3("lightPos", rc->position);
            
            material->Bind(true);

            // 绘制该材质下的所有MeshSlot
            for (const auto& meshslot : meshslots)
            {
                meshslot->mesh->Draw();
            }
        }
    }
}

void LightComponent::EnableShadow()
{
    if(!castShadow)
    {
        castShadow = true;
        auto shadowCamera = owner->AddComponent<CameraComponent>();
        auto tc = owner->GetComponent<TransformComponent>();
        glm::vec3 position = tc->position;
        switch (type)
        {
            case Type::Point:
            {
                shadowCamera->projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
                std::array<glm::mat4, 6> mview;
                mview[0] = glm::lookAt(position, position + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0));
                mview[1] = glm::lookAt(position, position + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0));
                mview[2] = glm::lookAt(position, position + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0));
                mview[3] = glm::lookAt(position, position + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0));
                mview[4] = glm::lookAt(position, position + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0));
                mview[5] = glm::lookAt(position, position + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0));
                shadowCamera->view = mview;
                break;
            }
            case Type::Directional:
            {
                shadowCamera->projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 15.0f);
                shadowCamera->view = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
                break;
            }
            case Type::Spot:
            {
                shadowCamera->projection = glm::perspective(glm::radians(theta), 1.0f, 1.0f, 15.0f);
                shadowCamera->view = glm::lookAt(position, position + direction, glm::vec3(0.0f, 1.0f, 0.0f));
                break;
            }
            default:
                break;
        }
    }
}
