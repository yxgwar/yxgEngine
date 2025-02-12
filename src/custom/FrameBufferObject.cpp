#include "FrameBufferObject.h"

FrameBufferObject::FrameBufferObject(int width, int height)
    :Object("../assets/shaders/frameScreen.vs", "../assets/shaders/frameScreen.fs")
{
    m_fbo.attachColor(width, height);
    m_fbo.attachDepthStencil(width, height);

    m_shader.use();
    m_shader.setInt("screenTexture", 0);

    float vertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    m_vbo.Init(vertices, sizeof(vertices));
    std::vector<VertexAttribute> attribute = {
        {0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0},
        {1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))}
    };

    m_vao.AddVBO(m_vbo, attribute);
}

void FrameBufferObject::StartDrawOnFrameBuffer()
{
    m_fbo.bind();
}

void FrameBufferObject::StartDrawFrameBuffer()
{
    m_fbo.unbind();
    m_fbo.bindTexture();

    glDisable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Draw();

    glEnable(GL_DEPTH_TEST);
}
