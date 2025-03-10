#pragma once

#include "ModelTest.h"

class Refract: public ModelTest
{
public:
    Refract(std::shared_ptr<Model> model);
    ~Refract() = default;
private:
    void PreProcess() override;
};