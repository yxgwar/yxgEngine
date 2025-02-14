#pragma once

#include "ModelTest.h"
#include "glfw/glfw3.h"

class Explore: public ModelTest
{
public:
    Explore(std::shared_ptr<Model> model);
    ~Explore() = default;
private:
    void PreProcess() override;
};