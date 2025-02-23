#pragma once

#include "glad/glad.h"

class IndexBuffer
{
public:
    IndexBuffer() = default;
    IndexBuffer(unsigned int* indices, size_t size);
    ~IndexBuffer();

    void Init(unsigned int* indices, size_t size);

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