#include "TestObject.h"

TestObject::TestObject(std::shared_ptr<Model> model)
    :ModelTest("../assets/shaders/test.vs", "../assets/shaders/test.fs")
{
    m_model = model;
}

void TestObject::OtherProcess()
{
    m_model->SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
}
