#include "vertexbuffer.h"
#include "glad/glad.h"
#include "mesh.h"

VertexBuffer::VertexBuffer(const void* vertices, size_t size)
{
    Init(vertices, size);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &ID);
}

void VertexBuffer::Init(const void *vertices, size_t size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    m_vertexCount = size / sizeof(Vertex);
}

void VertexBuffer::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
