#pragma once

#include "ModelTest.h"

class BlinnPhong: public ModelTest
{
public:
    BlinnPhong(std::shared_ptr<Model> model);
    ~BlinnPhong() = default;
private:
    void PreProcess() override;
};