#pragma once

#include <iostream>
#include "glad/glad.h"

class Texture
{
public:
    Texture(const char* path);
    ~Texture();

    void bind(int index);

    inline unsigned int getID() {return ID;}
private:
    unsigned int ID;
};