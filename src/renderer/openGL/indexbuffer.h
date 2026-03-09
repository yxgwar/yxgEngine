#pragma once

#include "glad/glad.h"

class IndexBuffer
{
public:
    IndexBuffer() = default;
    IndexBuffer(const unsigned int* indices, unsigned int count);
    ~IndexBuffer();

    void Init(const unsigned int* indices);

    void bind();
    void unbind();

    inline unsigned int getID() {return ID;}
    inline GLsizei getCount() {return count;}
    inline GLenum getType() {return type;}
private:
    unsigned int ID;
    GLsizei count;
    GLenum type;
};