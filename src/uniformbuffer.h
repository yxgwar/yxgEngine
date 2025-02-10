#pragma once

#include "glad/glad.h"

class UniformBuffer
{
public:
    UniformBuffer(GLsizeiptr size);
    ~UniformBuffer();

    void bind(unsigned int index, GLsizeiptr size);
    void setData(GLintptr offset, GLsizeiptr size, const void *data);
private:
    unsigned int ID;
};