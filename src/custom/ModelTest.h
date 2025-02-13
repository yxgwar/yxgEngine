#pragma once

#include "object/object.h"
#include "renderer/camera.h"
#include "object/model.h"

class ModelTest: public Object
{
public:
    ModelTest(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    virtual ~ModelTest() = default;

    void StartDraw(Camera& camera);

    void SetPosition(glm::mat4 model);
    inline glm::mat4 GetModelMatrix() {return m_modelM;}
    inline glm::mat3 GetNormalMatrix() {return m_NormalM;}
protected:
    virtual void PreProcess() = 0;
    std::shared_ptr<Model> m_model;
    glm::mat4 m_modelM;
    glm::mat3 m_NormalM;
};