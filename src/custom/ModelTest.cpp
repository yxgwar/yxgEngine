#include "ModelTest.h"

ModelTest::ModelTest(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
    :Object(vertexPath, fragmentPath, geometryPath)
{

}

void ModelTest::StartDraw(Camera &camera)
{
    m_shader.use();
    OtherProcess();
    m_shader.setVec3("viewPos", camera.getPosition());
    m_shader.setMat4("model", m_model->getModel());
    m_shader.setMat4("NormalM", m_model->getNormalM());
    m_model->Draw(m_shader);
}
