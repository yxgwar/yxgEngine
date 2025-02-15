#include "renderer.h"

void Renderer::Init(int width, int height)
{
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    m_width = width;
    m_height = height;
    
    //depthMap
    m_depthMap = std::make_unique<FrameBuffer>();
    m_depthMap->attachDepth(2048, 2048);

    //screen
    m_screen = std::make_unique<FrameBuffer>();
    m_screen->attachColor(width, height);
    m_screen->attachDepthStencil(width, height);
    m_screenShader = std::make_shared<Shader>("../assets/shaders/frameScreen.vs", "../assets/shaders/frameScreen.fs");
    m_screenShader->use();
    m_screenShader->setInt("screenTexture", 0);
}

void Renderer::SetViewportSize(int width, int height)
{
    glViewport(0, 0, width, height);
    m_width = width;
    m_height = height;
}

void Renderer::StartDrawDepthMap()
{
    if(m_depthMap)
    {
        glViewport(0, 0, m_depthMap->GetWidth(), m_depthMap->GetHeight());
        m_depthMap->bind();
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    else
        std::cout << "depthMap Uninit!" << std::endl;
}

void Renderer::EndDrawDepthMap()
{
    if(m_depthMap)
    {
        m_depthMap->unbind();
        m_depthMap->bindTexture();
    }
    else
        std::cout << "depthMap Uninit!" << std::endl;
}

void Renderer::StartRender()
{
    if(m_screen)
    {
        glViewport(0, 0, m_width, m_height);
        m_screen->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    else
        std::cout << "screenbuffer Uninit!" << std::endl;
}

void Renderer::EndRender()
{
    if(m_screen)
    {
        m_screen->unbind();
        m_screen->bindTexture();

        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        RenderQuad::DrawwithShader(*m_screenShader);

        glEnable(GL_DEPTH_TEST);
    }
    else
        std::cout << "screenbuffer Uninit!" << std::endl;
}

void Renderer::InitSkybox(std::vector<std::string> &faces)
{
    m_skybox = std::make_unique<TextureCube>(faces);
    m_skyboxShader = std::make_shared<Shader>("../assets/shaders/skybox/skybox.vs", "../assets/shaders/skybox/skybox.fs");
    m_skyboxShader->use();
    m_skyboxShader->setInt("skybox", 0);
}

void Renderer::DrawSkybox()
{
    if(m_skybox)
    {
        glDepthFunc(GL_LEQUAL);
        glCullFace(GL_FRONT);
        m_skybox->bind();
        RenderCube::DrawwithShader(*m_skyboxShader);
        glCullFace(GL_BACK);
        glDepthFunc(GL_LESS);
    }
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
