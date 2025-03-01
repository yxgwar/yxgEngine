#include "component.h"
#include "entity.h"

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

void RenderComponent::Render(Camera &camera) const
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
        
        // 遍历每个材质组
        for (const auto& [index, meshslots] : materialGroups)
        {
            const auto& material = materials[index];

            // 绑定材质并设置全局参数（如相机）
            material->SetVector3("viewPos", camera.getPosition());
            material->SetMatrix4("model", entityMatrix);
            material->SetMatrix3("NormalM", glm::transpose(glm::inverse(glm::mat3(entityMatrix))));
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