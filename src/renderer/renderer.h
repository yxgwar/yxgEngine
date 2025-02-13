#pragma once

#include "glad/glad.h"
#include "framebuffer.h"
#include "object/FrameBufferObject.h"
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

    //Stencil
    static void EnableStencil();
    static void DisableStencilWrite();
    static void DrawOnetoStencil();
    static void UseStencil();
    static void StopUseStencil();
private:
    inline static std::unique_ptr<FrameBuffer> m_depthMap = nullptr;
    inline static std::unique_ptr<FrameBufferObject> m_fbo = nullptr;
};