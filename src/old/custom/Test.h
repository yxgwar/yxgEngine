#pragma once

#include "ModelTest.h"

class Test: public ModelTest
{
public:
    Test(std::shared_ptr<Model> model);
    ~Test() = default;
private:
    void PreProcess() override;
};