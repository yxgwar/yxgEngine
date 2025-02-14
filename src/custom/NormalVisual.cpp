#include "NormalVisual.h"

NormalVisual::NormalVisual(std::shared_ptr<Model> model)
    :ModelTest("NormalVisual")
{
    m_model = model;

    SetDefaultShaderFromFile("../assets/shaders/normalVisual/normalVisual.vs", "../assets/shaders/normalVisual/normalVisual.fs", "../assets/shaders/normalVisual/normalVisual.gs");

    SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
}

void NormalVisual::PreProcess()
{
}
