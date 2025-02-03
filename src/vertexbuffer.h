#pragma once

#include "glad/glad.h"

class VertexBuffer
{
public:
    VertexBuffer(float* vertices, size_t size);
    ~VertexBuffer();

    void bind();
    void unbind();

    inline unsigned int getID() {return ID;}
    inline int getVertexCount() {return m_vertexCount;}
private:
    unsigned int ID;
    int m_vertexCount;
};