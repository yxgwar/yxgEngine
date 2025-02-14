#include "Refract.h"

Refract::Refract(std::shared_ptr<Model> model)
    :ModelTest("Refract")
{
    m_model = model;

    SetDefaultShaderFromFile("../assets/shaders/reflect.vs", "../assets/shaders/refract.fs");

    m_shader->use();
    m_shader->setInt("skybox", 0);

    SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
}

void Refract::PreProcess()
{

}
