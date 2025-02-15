#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    Shader() = default;
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    ~Shader();
    void use();

    // void Init(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    inline unsigned int getID() {return ID;}

    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setVec2(const std::string &name, float x, float y) const;
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    void setMat3(const std::string &name, glm::mat3 mat) const;
    void setMat4(const std::string &name, glm::mat4 mat) const;
    
    bool reload();
private:
    bool loadFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    bool checkError(GLuint shader, std::string type);
private:
    unsigned int ID;
    std::string m_vertexPath, m_fragmentPath;
};