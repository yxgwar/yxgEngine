#include "RenderQuad.h"
#include <iostream>

void RenderQuad::Init()
{
    float vertices[] = {
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // 右上角
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // 右下角
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下角
        -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f   // 左上角
    };

    unsigned int indices[] = {
        0, 2, 1, // 第一个三角形
        0, 3, 2  // 第二个三角形
    };

    m_vbo = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    m_ebo = std::make_shared<IndexBuffer>(indices, sizeof(indices));
    m_vao = std::make_unique<VertexArray>();

    std::vector<VertexAttribute> attribute = {
        {0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0},
        {1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))},
        {2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))}
    };

    m_vao->AddVBO(*m_vbo, attribute);
    m_vao->SetEBO(*m_ebo);
}

void RenderQuad::Draw()
{
    
}

void RenderQuad::DrawwithShader(Shader &shader)
{
    if(m_vao)
    {
        shader.use();
        m_vao->Draw();
    }
    else
        std::cout << "RenderQuad Uninit!" << std::endl;
}
