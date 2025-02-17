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
    void attachDepth(int width, int height);
    void attachMultiple(int width, int height);
    void attachDepthCube(int width, int height);

    void bindTexture();
    void bindTexture(int index);

    void bind();
    void unbind();

    inline int GetWidth() {return m_width;}
    inline int GetHeight() {return m_height;}
private:
    unsigned int ID;
    std::vector<unsigned int> m_texColorBuffers;
    unsigned int m_rbo = 0;
    int m_width, m_height;
};