#include "Test.h"

Test::Test(std::shared_ptr<Model> model)
    :ModelTest("Test")
{
    m_model = model;

    SetDefaultShaderFromFile("../assets/shaders/test.vs", "../assets/shaders/test.fs");

    SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
}

void Test::PreProcess()
{

}
