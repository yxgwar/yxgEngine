#pragma once

#include "ModelTest.h"

class EmptyModel: public ModelTest
{
public:
    EmptyModel(std::shared_ptr<Model> model);
    ~EmptyModel() = default;
private:
    void PreProcess() override;
};