#include "ModelTest.h"

ModelTest::ModelTest(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
    :Object(vertexPath, fragmentPath, geometryPath), m_modelM(glm::mat4(1.0f)), m_NormalM(glm::mat3(1.0f))
{

}

void ModelTest::StartDraw(Camera &camera)
{
    m_shader.use();
    PreProcess();
    m_shader.setVec3("viewPos", camera.getPosition());
    m_shader.setMat4("model", GetModelMatrix());
    m_shader.setMat3("NormalM", GetNormalMatrix());
    m_model->Draw(m_shader);
}

void ModelTest::StartDrawwithTempShader(Camera &camera, Shader &shader)
{
    shader.use();
    PreProcess();
    m_shader.setVec3("viewPos", camera.getPosition());
    m_shader.setMat4("model", GetModelMatrix());
    m_shader.setMat3("NormalM", GetNormalMatrix());
    m_model->Draw(shader);
}

void ModelTest::SetPosition(glm::mat4 model)
{
    m_modelM = model;
    m_NormalM = glm::transpose(glm::inverse(glm::mat3(m_modelM)));
}
