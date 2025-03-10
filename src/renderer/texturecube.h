#pragma once

#include <vector>
#include <string>

class TextureCube
{
public:
    TextureCube() = default;
    TextureCube(std::vector<std::string>& faces);
    ~TextureCube();

    void Init(std::vector<std::string>& faces);

    void bind();

    inline unsigned int getID() {return ID;}
private:
    unsigned int ID;
};