#pragma once

#include <iostream>
#include <vector>
#include "glad/glad.h"

class TextureCube
{
public:
    TextureCube(std::vector<std::string> faces);
    ~TextureCube();

    void bind();

    inline unsigned int getID() {return ID;}
private:
    unsigned int ID;
};