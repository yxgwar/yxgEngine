#pragma once

#include "renderer/openGL/texture.h"
#include "renderer/openGL/shader.h"
#include <vector>
#include <memory>

// 纹理信息结构体
struct TextureInfo
{
    std::shared_ptr<Texture> tex;   // 纹理ID
    std::string type;               // 纹理类型（如"texture_diffuse", "texture_normal"）
    std::string path;               // 纹理文件路径（可选，用于调试）
};

class Material
{
public:
    Material(std::shared_ptr<Shader> shader);
    ~Material() = default;

    // 设置Uniform值
    void SetInt(const std::string& name, int value);
    void SetFloat(const std::string& name, float value);
    void SetVector3(const std::string& name, const glm::vec3& value);
    void SetMatrix3(const std::string& name, const glm::mat3& value);
    void SetMatrix4(const std::string& name, const glm::mat4& value);

    // 绑定材质（在绘制前调用）
    void Bind(bool gBuffer = false) const;

    void AddTexture(std::shared_ptr<Texture> tex, const std::string& type, const std::string& path = std::string());
private:
    std::shared_ptr<Shader> gBuffershader;

    std::shared_ptr<Shader> m_shader;
    std::vector<TextureInfo> m_textures; // 纹理列表

    // Uniform缓存（避免重复设置）
    std::unordered_map<std::string, int> m_intUniforms;
    std::unordered_map<std::string, float> m_floatUniforms;
    std::unordered_map<std::string, glm::vec3> m_vec3Uniforms;
    std::unordered_map<std::string, glm::mat3> m_mat3Uniforms;
    std::unordered_map<std::string, glm::mat4> m_mat4Uniforms;
};