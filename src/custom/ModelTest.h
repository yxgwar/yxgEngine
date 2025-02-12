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
protected:
    virtual void OtherProcess() = 0;
    std::shared_ptr<Model> m_model;
};