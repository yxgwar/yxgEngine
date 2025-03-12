#pragma once

#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    Texture();
    Texture(int width, int height, void* data);
    ~Texture();

    void bind(int index);

    inline unsigned int getID() {return ID;}
private:
    void load(const char* path);
    unsigned int ID;
};