#include "TestObject.h"

TestObject::TestObject(std::shared_ptr<Model> model)
    :ModelTest("../assets/shaders/test.vs", "../assets/shaders/test.fs")
{
    m_model = model;
    SetPosition(glm::mat4(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 0.0f, 0.0f))));
}

void TestObject::PreProcess()
{

}
