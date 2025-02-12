#pragma once

#include "renderer/vertexarray.h"
#include "renderer/vertexbuffer.h"
#include "renderer/indexbuffer.h"
#include "renderer/shader.h"

class Object
{
public:
    Object(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    virtual ~Object() = default;
protected:
    void Draw();
    VertexArray m_vao;
    VertexBuffer m_vbo;
    IndexBuffer m_ebo;
    Shader m_shader;
};