#pragma once

#include "ModelTest.h"

class Reflect: public ModelTest
{
public:
    Reflect(std::shared_ptr<Model> model);
    ~Reflect() = default;
private:
    void PreProcess() override;
};