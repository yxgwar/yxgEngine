#pragma once

#include <iostream>
#include "glad/glad.h"

class Texture
{
public:
    Texture(const char* path, std::string& type);
    ~Texture();

    void bind(int index);

    inline unsigned int getID() {return ID;}
    inline std::string getType() {return m_type;}
    inline std::string getPath() {return m_path;}
private:
    unsigned int ID;
    std::string m_type;
    std::string m_path;
};