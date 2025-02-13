#include "ExploreObject.h"

ExploreObject::ExploreObject(std::shared_ptr<Model> model)
    :ModelTest("../assets/shaders/explore/explore.vs", "../assets/shaders/explore/explore.fs", "../assets/shaders/explore/explore.gs")
{
    m_model = model;
    SetPosition(glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f))));
}

void ExploreObject::PreProcess()
{
    m_shader.setFloat("time", glfwGetTime());
}
