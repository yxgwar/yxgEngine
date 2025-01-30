#include "vertexbuffer.h"

VertexBuffer::VertexBuffer(float* vertices, size_t size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID); 
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &ID);
}

void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
