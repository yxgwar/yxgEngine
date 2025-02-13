#pragma once

#include "ModelTest.h"

class TestObject: public ModelTest
{
public:
    TestObject(std::shared_ptr<Model> model);
    ~TestObject() = default;
private:
    void PreProcess() override;
};