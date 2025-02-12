#pragma once

#include "glad/glad.h"

class Renderer
{
public:
    static void Init(int width, int height);
    static void RendererStart();

    //Stencil
    static void EnableStencil();
    static void DisableStencilWrite();
    static void DrawOnetoStencil();
    static void UseStencil();
    static void StopUseStencil();
private:
};