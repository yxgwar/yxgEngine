#pragma once

#include "ModelTest.h"
#include "glfw/glfw3.h"

class ExploreObject: public ModelTest
{
public:
    ExploreObject(std::shared_ptr<Model> model);
    ~ExploreObject() = default;
private:
    void OtherProcess() override;
};