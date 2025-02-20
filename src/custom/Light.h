#pragma once

#include "renderer/framebuffer.h"
#include "renderer/shader.h"
#include "renderer/renderer.h"
#include "Object.h"

class Light:public Object
{
public:
    Light(const char* name);
    virtual ~Light() = default;

    void Draw();
    virtual void StartDrawDepthMap(Shader& shader) = 0;
    void StopDrawDepthMap();
    void BindDepthMap(int index);
protected:
    virtual void DrawwithType() = 0;
    FrameBuffer m_depthMap;
    Shader m_shader;
    int m_width = 4096, m_height = 4096;
};