#pragma once

#include "renderer/camera.h"
#include "object/model.h"
#include "Object.h"

class ModelTest: public Object
{
public:
    ModelTest(const char* name);
    virtual ~ModelTest() = default;

    void StartDraw(Camera& camera);
    void StartDrawwithShader(Camera& camera, Shader& shader);

    inline void SetDefaultShader(std::shared_ptr<Shader> shader) {m_shader = shader;}
    void SetDefaultShaderFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
protected:
    virtual void PreProcess() = 0;
    std::shared_ptr<Model> m_model;
    std::shared_ptr<Shader> m_shader;
};