#include "material.h"

Material::Material(std::shared_ptr<Shader> shader)
    :m_shader(shader)
{
}

void Material::SetInt(const std::string &name, int value)
{
    m_intUniforms[name] = value;
}

void Material::SetFloat(const std::string &name, float value)
{
    m_floatUniforms[name] = value;
}

void Material::SetVector3(const std::string &name, const glm::vec3 &value)
{
    m_vec3Uniforms[name] = value;
}

void Material::SetMatrix3(const std::string &name, const glm::mat3 &value)
{
    m_mat3Uniforms[name] = value;
}

void Material::SetMatrix4(const std::string &name, const glm::mat4 &value)
{
    m_mat4Uniforms[name] = value;
}

void Material::Bind() const
{
    m_shader->use();

    for(int i = 0; i < m_textures.size(); i++)
    {
        m_textures[i].tex->bind(i);
        m_shader->setInt(m_textures[i].type, i);
    }

    // 设置Uniform值
    for (const auto& [name, value] : m_intUniforms)
        m_shader->setInt(name, value);
    for (const auto& [name, value] : m_floatUniforms)
        m_shader->setFloat(name, value);
    for (const auto& [name, value] : m_vec3Uniforms)
        m_shader->setVec3(name, value);
    for (const auto& [name, value] : m_mat3Uniforms)
        m_shader->setMat3(name, value);
    for (const auto& [name, value] : m_mat4Uniforms)
        m_shader->setMat4(name, value);
}

void Material::AddTexture(std::shared_ptr<Texture> tex, const std::string &type, const std::string &path)
{
    m_textures.emplace_back(TextureInfo{tex, type, path});
}
