#pragma once

#include "renderer/camera.h"
#include "object/model.h"

class ModelTest
{
public:
    ModelTest(const char* name);
    virtual ~ModelTest() = default;

    void StartDraw(Camera& camera);
    void StartDrawwithShader(Camera& camera, Shader& shader);

    inline void SetDefaultShader(std::shared_ptr<Shader> shader) {m_shader = shader;}
    void SetDefaultShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    void SetPosition(glm::vec3 pos);
    inline glm::mat4 GetModelMatrix() {return m_modelM;}
    inline glm::mat3 GetNormalMatrix() {return m_NormalM;}
protected:
    virtual void PreProcess() = 0;
    std::shared_ptr<Model> m_model;
    std::shared_ptr<Shader> m_shader;
    glm::mat4 m_modelM;
    glm::mat3 m_NormalM;
    std::string m_name;
};