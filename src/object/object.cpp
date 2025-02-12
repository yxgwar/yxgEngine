#include "object.h"

Object::Object(const char *vertexPath, const char *fragmentPath, const char *geometryPath)
    :m_shader(vertexPath, fragmentPath, geometryPath)
{
}

void Object::Draw()
{
    m_shader.use();
    m_vao.Draw();
}