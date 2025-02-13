#pragma once

#include "ModelTest.h"

class NormalVisualObject: public ModelTest
{
public:
    NormalVisualObject(std::shared_ptr<Model> model);
    ~NormalVisualObject() = default;
private:
    void PreProcess() override;
};