#include "renderer.h"

void Renderer::Init(int width, int height)
{
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    m_depthMap = std::make_unique<FrameBuffer>();
    m_fbo = std::make_unique<FrameBufferObject>(width, height);
    m_depthMap->attachDepth(1024, 1024);
}

void Renderer::SetViewportSize(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Renderer::StartDrawDepthMap()
{
    glViewport(0, 0, m_depthMap->GetWidth(), m_depthMap->GetHeight());
    m_depthMap->bind();
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Renderer::EndDrawDepthMap()
{
    m_depthMap->unbind();
}

void Renderer::StartRender()
{
    m_fbo->StartDrawOnFrameBuffer();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::EndRender()
{
    m_fbo->StartDrawFrameBuffer();
}

void Renderer::EnableStencil()
{
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}

void Renderer::DisableStencilWrite()
{
    glStencilMask(0x00);
}

void Renderer::DrawOnetoStencil()
{
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
}

void Renderer::UseStencil()
{
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);
}

void Renderer::StopUseStencil()
{
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glEnable(GL_DEPTH_TEST);
}
