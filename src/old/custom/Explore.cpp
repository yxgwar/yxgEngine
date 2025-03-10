#include "Explore.h"

Explore::Explore(std::shared_ptr<Model> model)
    :ModelTest("Explore")
{
    m_model = model;

    SetDefaultShaderFromFile("../assets/shaders/explore/explore.vs", "../assets/shaders/explore/explore.fs", "../assets/shaders/explore/explore.gs");

    SetPosition(glm::vec3(10.0f, 0.0f, 0.0f));
}

void Explore::PreProcess()
{
    if(m_shader)
    {
        m_shader->setFloat("time", glfwGetTime());
    }
    else
        std::cout << m_name << ":Default Shader not Set!" << std::endl;
}
