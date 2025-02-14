#pragma once

#include "glad/glad.h"
#include "framebuffer.h"
#include "RenderQuad.h"
#include "RenderCube.h"
#include "texturecube.h"
#include <memory>

class Renderer
{
public:
    static void Init(int width, int height);
    static void SetViewportSize(int width, int height);

    //DepthMap
    static void StartDrawDepthMap();
    static void EndDrawDepthMap();

    //Screen
    static void StartRender();
    static void EndRender();

    //skybox
    static void InitSkybox(std::vector<std::string>& faces);
    static void DrawSkybox();

    //Stencil
    static void EnableStencil();
    static void DisableStencilWrite();
    static void DrawOnetoStencil();
    static void UseStencil();
    static void StopUseStencil();
private:
    inline static std::unique_ptr<FrameBuffer> m_depthMap = nullptr;
    inline static std::unique_ptr<FrameBuffer> m_screen = nullptr;
    inline static std::shared_ptr<Shader> m_screenShader = nullptr;
    inline static int m_width = 0, m_height = 0;

    inline static std::unique_ptr<TextureCube> m_skybox = nullptr;
    inline static std::shared_ptr<Shader> m_skyboxShader = nullptr;
};