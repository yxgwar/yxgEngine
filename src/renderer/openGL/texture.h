#pragma once

#include <string>

class Texture
{
public:
    Texture(const std::string& path);
    Texture(const std::string& directory, const std::string& path, std::string& type);
    Texture();
    ~Texture();

    void bind(int index);

    inline unsigned int getID() {return ID;}
    inline std::string getType() {return m_type;}
    inline std::string getPath() {return m_path;}
private:
    void load(const char* path);
    unsigned int ID;
    std::string m_type;
    std::string m_path;
};