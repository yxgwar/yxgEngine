#pragma once

#include "ModelTest.h"

class RefractObject: public ModelTest
{
public:
    RefractObject(std::shared_ptr<Model> model);
    ~RefractObject() = default;
private:
    void PreProcess() override;
};