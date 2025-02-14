#include "Reflect.h"

Reflect::Reflect(std::shared_ptr<Model> model)
    :ModelTest("Reflect")
{
    m_model = model;

    SetDefaultShaderFromFile("../assets/shaders/reflect.vs", "../assets/shaders/reflect.fs");
    
    m_shader->use();
    m_shader->setInt("skybox", 0);

    SetPosition(glm::vec3(5.0f, 0.0f, 0.0f));
}

void Reflect::PreProcess()
{

}
