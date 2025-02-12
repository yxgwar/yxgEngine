#include "ExploreObject.h"

ExploreObject::ExploreObject(std::shared_ptr<Model> model)
    :ModelTest("../assets/shaders/explore/explore.vs", "../assets/shaders/explore/explore.fs", "../assets/shaders/explore/explore.gs")
{
    m_model = model;
}

void ExploreObject::OtherProcess()
{
    m_model->SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
    m_shader.setFloat("time", glfwGetTime());
}
