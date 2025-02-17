#include "Light.h"

Light::Light(const char *name)
    :Object(name), m_shader("../assets/shaders/White/white.vs", "../assets/shaders/White/white.fs")
{
    SetScale(glm::vec3(0.1f));
}

void Light::Draw()
{
    m_shader.use();
    m_shader.setMat4("model", GetModelMatrix());
    DrawwithType();
}

void Light::StopDrawDepthMap()
{
    m_depthMap.unbind();
}

void Light::BindDepthMap(int index)
{
    m_depthMap.bindTexture(index);
}
