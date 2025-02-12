#pragma once

#include "ModelTest.h"

class BlinnPhongObject: public ModelTest
{
public:
    BlinnPhongObject(std::shared_ptr<Model> model);
    ~BlinnPhongObject() = default;
private:
    void OtherProcess() override;
};