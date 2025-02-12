#pragma once

#include "object/object.h"
#include "renderer/shader.h"
#include "renderer/texturecube.h"

class SkyboxObject: public Object
{
public:
    SkyboxObject();
    ~SkyboxObject() = default;

    void StartDrawSkybox();
private:
    TextureCube m_skybox;
};