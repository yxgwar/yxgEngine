#pragma once

#include <cstddef>

class VertexBuffer
{
public:
    VertexBuffer() = default;
    VertexBuffer(float* vertices, size_t size);
    ~VertexBuffer();

    void Init(float* vertices, size_t size);

    void bind();
    void unbind();

    inline unsigned int getID() {return ID;}
    inline int getVertexCount() {return m_vertexCount;}
private:
    unsigned int ID;
    int m_vertexCount;
};