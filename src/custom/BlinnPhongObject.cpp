#include "BlinnPhongObject.h"

BlinnPhongObject::BlinnPhongObject(std::shared_ptr<Model> model)
    :ModelTest("../assets/shaders/model.vs", "../assets/shaders/model.fs")
{
    m_model = model;
    glm::vec3 pointLightPositions = glm::vec3( 0.7f,  0.2f,  2.0f);
    m_shader.use();
    m_shader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
    m_shader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
    m_shader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
    m_shader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
    // point light 1
    m_shader.setVec3("pointLights.position", pointLightPositions);
    m_shader.setVec3("pointLights.ambient", 0.05f, 0.05f, 0.05f);
    m_shader.setVec3("pointLights.diffuse", 0.8f, 0.8f, 0.8f);
    m_shader.setVec3("pointLights.specular", 1.0f, 1.0f, 1.0f);
    m_shader.setFloat("pointLights.constant", 1.0f);
    m_shader.setFloat("pointLights.linear", 0.09f);
    m_shader.setFloat("pointLights.quadratic", 0.032f);
}

void BlinnPhongObject::OtherProcess()
{
    m_model->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
}
