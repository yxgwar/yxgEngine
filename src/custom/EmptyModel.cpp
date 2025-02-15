#include "EmptyModel.h"

EmptyModel::EmptyModel(std::shared_ptr<Model> model)
    :ModelTest("EmptyModel")
{
    m_model = model;
}

void EmptyModel::PreProcess()
{
}
