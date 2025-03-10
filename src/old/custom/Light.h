#pragma once

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

    inline void SetColor(glm::vec3 color) {m_color = color;}
    inline glm::vec3 GetColor() {return m_color;}
protected:
    virtual void DrawwithType() = 0;
    FrameBuffer m_depthMap;
    Shader m_shader;
    glm::vec3 m_color;
    int m_width = 4096, m_height = 4096;
};