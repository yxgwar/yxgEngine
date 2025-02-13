#pragma once

#include "ModelTest.h"

class ReflectObject: public ModelTest
{
public:
    ReflectObject(std::shared_ptr<Model> model);
    ~ReflectObject() = default;
private:
    void PreProcess() override;
};