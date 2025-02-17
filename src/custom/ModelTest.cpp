#include "ModelTest.h"

ModelTest::ModelTest(const char* name)
    :Object(name)
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