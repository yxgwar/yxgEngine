#include "PointLight.h"

PointLight::PointLight()
    :Light("Point Light"), m_vp(6, glm::mat4(1.0f))
{
    m_depthMap.attachDepthCube(m_width, m_height);
    m_projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
    calcuVP();
}

void PointLight::SetPosition(glm::vec3 pos)
{
    Object::SetPosition(pos);
    calcuVP();
}

void PointLight::StartDrawDepthMap(Shader& shader)
{
    glViewport(0, 0, m_width, m_height);
    m_depthMap.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    shader.use();
    for (GLuint i = 0; i < 6; ++i)
        shader.setMat4("shadowMatrices[" + std::to_string(i) + "]", m_vp[i]);
    shader.setFloat("far_plane", 100.0f);
    shader.setVec3("lightPos", m_position);
}

void PointLight::calcuVP()
{
    m_vp[0] = m_projection * glm::lookAt(m_position, m_position + glm::vec3( 1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0));
    m_vp[1] = m_projection * glm::lookAt(m_position, m_position + glm::vec3(-1.0,  0.0,  0.0), glm::vec3(0.0, -1.0,  0.0));
    m_vp[2] = m_projection * glm::lookAt(m_position, m_position + glm::vec3( 0.0,  1.0,  0.0), glm::vec3(0.0,  0.0,  1.0));
    m_vp[3] = m_projection * glm::lookAt(m_position, m_position + glm::vec3( 0.0, -1.0,  0.0), glm::vec3(0.0,  0.0, -1.0));
    m_vp[4] = m_projection * glm::lookAt(m_position, m_position + glm::vec3( 0.0,  0.0,  1.0), glm::vec3(0.0, -1.0,  0.0));
    m_vp[5] = m_projection * glm::lookAt(m_position, m_position + glm::vec3( 0.0,  0.0, -1.0), glm::vec3(0.0, -1.0,  0.0));
}

void PointLight::DrawwithType()
{
    RenderCube::DrawwithShader(m_shader);
}
