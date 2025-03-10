#include "vertexarray.h"
#include "glad/glad.h"

VertexArray::VertexArray()
    :m_count(0), m_type(0), m_vertexCount(0)
{
    glGenVertexArrays(1, &ID);
    glBindVertexArray(ID);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &ID);
}

void VertexArray::AddVBO(VertexBuffer &vb, std::vector<VertexAttribute> &attribute)
{
    bind();
    vb.bind();

    for(auto& attr: attribute)
    {
        glEnableVertexAttribArray(attr.index);
        if(attr.type == GL_FLOAT)
            glVertexAttribPointer(
                attr.index,
                attr.size,
                attr.type,
                attr.normalized,
                attr.stride,
                attr.pointer
            );
        else if(attr.type == GL_INT)
            glVertexAttribIPointer(
                attr.index,
                attr.size,
                attr.type,
                attr.stride,
                attr.pointer
            );
    }

    m_vertexCount += vb.getVertexCount();

    vb.unbind();
    unbind();
}

void VertexArray::SetEBO(IndexBuffer &ib)
{
    bind();
    ib.bind();

    m_count = ib.getCount();
    m_type = ib.getType();

    unbind();
    ib.unbind();
}

void VertexArray::Draw()
{
    bind();
    if(m_count > 0)
        glDrawElements(GL_TRIANGLES, m_count, m_type, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
    unbind();
}

void VertexArray::bind()
{
    glBindVertexArray(ID);
}

void VertexArray::unbind()
{
    glBindVertexArray(0);
}
