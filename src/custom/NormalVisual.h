#pragma once

#include "ModelTest.h"

class NormalVisual: public ModelTest
{
public:
    NormalVisual(std::shared_ptr<Model> model);
    ~NormalVisual() = default;
private:
    void PreProcess() override;
};