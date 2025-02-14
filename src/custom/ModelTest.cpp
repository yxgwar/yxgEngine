#include "ModelTest.h"

ModelTest::ModelTest(const char* name)
    :m_modelM(glm::mat4(1.0f)), m_NormalM(glm::mat3(1.0f)), m_name(name)
{

}

void ModelTest::StartDraw(Camera &camera)
{
    if(m_shader)
    {
        m_shader->use();
        PreProcess();
        m_shader->setVec3("viewPos", camera.getPosition());
        m_shader->setMat4("model", GetModelMatrix());
        m_shader->setMat3("NormalM", GetNormalMatrix());
        m_model->Draw(*m_shader);
    }
    else
        std::cout << m_name << ":No default Shader!" << std::endl;
}

void ModelTest::StartDrawwithShader(Camera &camera, Shader &shader)
{
    shader.use();
    PreProcess();
    shader.setVec3("viewPos", camera.getPosition());
    shader.setMat4("model", GetModelMatrix());
    shader.setMat3("NormalM", GetNormalMatrix());
    m_model->Draw(shader);
}

void ModelTest::SetDefaultShaderFromFile(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
{
    m_shader = std::make_shared<Shader>(vertexPath, fragmentPath, geometryPath);
}

void ModelTest::SetPosition(glm::vec3 pos)
{
    m_modelM = glm::translate(glm::mat4(1.0f), pos);
    m_NormalM = glm::transpose(glm::inverse(glm::mat3(m_modelM)));
}