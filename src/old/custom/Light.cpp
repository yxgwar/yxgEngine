#include "Light.h"

Light::Light(const char *name)
    :Object(name), m_shader("../assets/shaders/Color/color.vs", "../assets/shaders/Color/color.fs"), m_color(1.0f, 1.0f, 1.0f)
{
}

void Light::Draw()
{
    m_shader.use();
    m_shader.setMat4("model", GetModelMatrix());
    m_shader.setVec3("color", m_color);
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
