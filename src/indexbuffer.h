#pragma once

#include "glad/glad.h"

class IndexBuffer
{
public:
    IndexBuffer(unsigned int* indices, size_t size);
    ~IndexBuffer();

    void bind();
    void unbind();

    inline unsigned int getID() {return ID;}
private:
    unsigned int ID;
};