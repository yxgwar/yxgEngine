#pragma once

#include "glad/glad.h"
#include "Light.h"

class PointLight :public Light 
{
public:
    PointLight();
    ~PointLight() = default;

    void SetPosition(glm::vec3 pos) override;
    void StartDrawDepthMap(Shader& shader) override;
private:
    void calcuVP();
    void DrawwithType();
    glm::mat4 m_projection;
    std::vector<glm::mat4> m_vp;
};