#pragma once

#include "vertexbuffer.h"
#include "indexbuffer.h"
#include <vector>

struct VertexAttribute
{
    GLuint index;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    const void *pointer;
};

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddVBO(VertexBuffer& vb, std::vector<VertexAttribute>& attribute);
    void SetEBO(IndexBuffer& ib);

    void Draw();

    void bind();
    void unbind();

    inline unsigned int getID() {return ID;}
private:
    unsigned int ID;
    int m_vertexCount;
    int m_count;
    unsigned int m_type;
};