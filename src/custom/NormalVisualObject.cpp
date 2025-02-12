#include "NormalVisualObject.h"

NormalVisualObject::NormalVisualObject(std::shared_ptr<Model> model)
    :ModelTest("../assets/shaders/normalVisual/normalVisual.vs", "../assets/shaders/normalVisual/normalVisual.fs", "../assets/shaders/normalVisual/normalVisual.gs")
{
    m_model = model;
}

void NormalVisualObject::OtherProcess()
{
}
