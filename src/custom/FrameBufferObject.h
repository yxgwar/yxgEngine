#pragma once

#include "object/object.h"
#include "renderer/framebuffer.h"

class FrameBufferObject: public Object
{
public:
    FrameBufferObject(int width, int height);
    ~FrameBufferObject() = default;

    void StartDrawOnFrameBuffer();
    void StartDrawFrameBuffer();
private:
    FrameBuffer m_fbo;
};