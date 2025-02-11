#pragma once

#include "glad/glad.h"
#include <vector>
#include <iostream>

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void attachColor(int width, int height);
    void attachDepthStencil(int width, int height);

    void bindTexture();

    void bind();
    void unbind();
private:
    unsigned int ID;
    std::vector<unsigned int> m_texColorBuffers;
    unsigned int m_rbo = 0;
};