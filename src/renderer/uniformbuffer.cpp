#include "uniformbuffer.h"

UniformBuffer::UniformBuffer(GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    glBufferData(GL_UNIFORM_BUFFER, size, nullptr, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &ID);
}

void UniformBuffer::bind(unsigned int index, GLsizeiptr size)
{
    glBindBufferRange(GL_UNIFORM_BUFFER, index, ID, 0, size);
}

void UniformBuffer::setData(GLintptr offset, GLsizeiptr size, const void *data)
{
    glBindBuffer(GL_UNIFORM_BUFFER, ID);
    glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
