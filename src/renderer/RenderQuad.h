#pragma once

#include "glad/glad.h"
#include "openGL/vertexarray.h"
#include "openGL/vertexbuffer.h"
#include "openGL/indexbuffer.h"
#include "openGL/shader.h"
#include <memory>

class RenderQuad
{
public:
    static void Init();
    static void Draw();
    static void DrawwithShader(Shader& shader);
private:
    inline static std::unique_ptr<VertexArray> m_vao = nullptr;
    inline static std::shared_ptr<VertexBuffer> m_vbo = nullptr;
    inline static std::shared_ptr<IndexBuffer> m_ebo = nullptr;
};