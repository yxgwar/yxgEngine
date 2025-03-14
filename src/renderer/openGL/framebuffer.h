#pragma once

#include <vector>

class FrameBuffer
{
public:
    FrameBuffer();
    ~FrameBuffer();

    void attachColor        (int width, int height);
    void attachDepthStencil (int width, int height);
    void attachDepth        (int width, int height);
    void attachMultiple     (int width, int height);
    void attachDepthCube    (int width, int height);
    void attachHDR          (int width, int height);
    void attachPingPong     (int width, int height);
    void attachgBuffer      (int width, int height);
    void attachSingle       (int width, int height);

    void bindTexture(int index = 0);
    void bindSingleTexture(int index);

    void bind();
    void bindRead();
    void bindDraw();
    void blitDepth();
    void unbind();

    inline int GetWidth() const {return m_width;}
    inline int GetHeight() const {return m_height;}
    inline unsigned int GetColorAttachmentID() const {return m_texColorBuffers[0];}
private:
    unsigned int ID;
    std::vector<unsigned int> m_texColorBuffers;
    unsigned int m_rbo = 0;
    int m_width, m_height;
};