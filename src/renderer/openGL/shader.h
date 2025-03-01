#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

class Shader
{
public:
    Shader() = default;
    Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = std::string());
    ~Shader();
    void use();

    // void Init(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

    inline GLuint getID() {return ID;}

    void setBool(const std::string &name, bool value);  
    void setInt(const std::string &name, int value);   
    void setFloat(const std::string &name, float value);
    void setVec2(const std::string &name, float x, float y);
    void setVec3(const std::string &name, const glm::vec3 &value);
    void setVec3(const std::string &name, float x, float y, float z);
    void setMat3(const std::string &name, glm::mat3 mat);
    void setMat4(const std::string &name, glm::mat4 mat);
private:
    bool loadFromFile(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = std::string());
    bool checkError(GLuint shader, std::string type);
private:
    GLuint ID;
    std::unordered_map<std::string, GLint> m_uniformCache; // Uniform位置缓存
    GLint getUniformLocation(const std::string& name);
};