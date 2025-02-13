#include "ReflectObject.h"

ReflectObject::ReflectObject(std::shared_ptr<Model> model)
    :ModelTest("../assets/shaders/reflect.vs", "../assets/shaders/reflect.fs")
{
    m_model = model;
    
    m_shader.use();
    m_shader.setInt("skybox", 0);

    SetPosition(glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 0.0f, 0.0f))));
}

void ReflectObject::PreProcess()
{

}
