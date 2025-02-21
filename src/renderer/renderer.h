#pragma once

#include "glad/glad.h"
#include "framebuffer.h"
#include "RenderQuad.h"
#include "RenderCube.h"
#include "texturecube.h"
#include "uniformbuffer.h"
#include "camera.h"
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

    //hdr
    static void StartRenderHDR();
    static void EndRenderHDR(float exposure);

    //skybox
    static void InitSkybox(std::vector<std::string>& faces);
    static void DrawSkybox();

    //Stencil
    static void EnableStencil();
    static void DisableStencilWrite();
    static void DrawOnetoStencil();
    static void UseStencil();
    static void StopUseStencil();

    //UBO
    static void SetCameraUBO(Camera& camera);
    static void UpdateCameraUBO(Camera& camera);
private:
    inline static std::unique_ptr<FrameBuffer> m_depthMap = nullptr;
    inline static std::unique_ptr<FrameBuffer> m_screen = nullptr;
    inline static std::shared_ptr<Shader> m_screenShader = nullptr;
    inline static std::unique_ptr<FrameBuffer> m_hdr = nullptr;
    inline static std::shared_ptr<Shader> m_hdrShader = nullptr;
    inline static int m_width = 0, m_height = 0;

    inline static std::unique_ptr<TextureCube> m_skybox = nullptr;
    inline static std::shared_ptr<Shader> m_skyboxShader = nullptr;

    inline static std::unique_ptr<UniformBuffer> m_ubo = nullptr;
};