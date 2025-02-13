#include "NormalVisualObject.h"

NormalVisualObject::NormalVisualObject(std::shared_ptr<Model> model)
    :ModelTest("../assets/shaders/normalVisual/normalVisual.vs", "../assets/shaders/normalVisual/normalVisual.fs", "../assets/shaders/normalVisual/normalVisual.gs")
{
    m_model = model;
    SetPosition(glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f))));
}

void NormalVisualObject::PreProcess()
{
}
