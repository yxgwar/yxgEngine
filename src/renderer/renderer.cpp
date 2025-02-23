#include "renderer.h"

void Renderer::Init(int width, int height)
{
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    // glEnable(GL_CULL_FACE);

    m_width = width;
    m_height = height;
    
    //depthMap
    m_depthMap = std::make_unique<FrameBuffer>();
    m_depthMap->attachDepth(4096, 4096);

    //screen
    m_screen = std::make_unique<FrameBuffer>();
    // m_screen->attachColor(width, height);
    // m_screen->attachDepthStencil(width, height);
    m_screen->attachMultiple(width, height);
    m_screenShader = std::make_shared<Shader>("../assets/shaders/Screen/frameScreen.vs", "../assets/shaders/Screen/frameScreen.fs");
    m_screenShader->use();
    m_screenShader->setInt("screenTexture", 0);
    m_screenShader->setInt("samples", 4);
    m_screenShader->setVec2("textureSize", width, height);

    //hdr
    m_hdr = std::make_unique<FrameBuffer>();
    m_hdr->attachHDR(width, height);
    m_hdrShader = std::make_shared<Shader>("../assets/shaders/HDR/hdr.vs", "../assets/shaders/HDR/hdr.fs");
    m_hdrShader->use();
    m_hdrShader->setInt("scene", 0);
    m_hdrShader->setInt("bloomBlur", 1);
    m_hdrShader->setFloat("exposure", 1.0f);

    //bloom
    m_blurH = std::make_unique<FrameBuffer>();
    m_blurH->attachPingPong(width, height);
    m_blurV = std::make_unique<FrameBuffer>();
    m_blurV->attachPingPong(width, height);
    m_blurShader = std::make_shared<Shader>("../assets/shaders/HDR/blur.vs", "../assets/shaders/HDR/blur.fs");
    m_blurShader->use();
    m_blurShader->setInt("image", 0);
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
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

void Renderer::StartRenderHDR()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    if(m_hdr)
    {
        glViewport(0, 0, m_width, m_height);
        m_hdr->bind();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    else
        std::cout << "screenbuffer Uninit!" << std::endl;
}

void Renderer::EndRenderHDR(float exposure, unsigned int ID)
{
    if(m_hdr)
    {
        m_hdr->unbind();
        m_hdr->bindSingleTexture(1);

        bool horizontal = true;
        unsigned int amount = 10;
        m_blurShader->use();
        for (unsigned int i = 0; i <= amount; i++)
        {
            if(horizontal)
                m_blurH->bind();
            else
                m_blurV->bind();
            m_blurShader->setInt("horizontal", horizontal);
            RenderQuad::DrawwithShader(*m_blurShader);
            if(horizontal)
                m_blurH->bindTexture();
            else
                m_blurV->bindTexture();
            
            horizontal = !horizontal;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, ID);

        // glDisable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_hdr->bindSingleTexture(0);
        if(!horizontal)
            m_blurH->bindTexture(1);
        else
            m_blurV->bindTexture(1);

        m_hdrShader->use();
        m_hdrShader->setFloat("exposure", exposure);
        RenderQuad::DrawwithShader(*m_hdrShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // glEnable(GL_DEPTH_TEST);
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

void Renderer::SetCameraUBO(Camera &camera)
{
    m_ubo = std::make_unique<UniformBuffer>(2 * sizeof(glm::mat4));
    m_ubo->bind(0, 2 * sizeof(glm::mat4));
    m_ubo->setData(0, sizeof(glm::mat4), glm::value_ptr(camera.getProjection()));
}

void Renderer::UpdateCameraUBO(Camera &camera)
{
    m_ubo->setData(sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(camera.getView()));
}
