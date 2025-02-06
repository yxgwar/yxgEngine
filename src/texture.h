#pragma once

#include <iostream>
#include "glad/glad.h"

class Texture
{
public:
    Texture(const char* path, const char* type = "texture_diffuse");
    ~Texture();

    void bind(int index);

    inline unsigned int getID() {return ID;}
    inline std::string getType() {return m_type;}
private:
    unsigned int ID;
    std::string m_type;
};