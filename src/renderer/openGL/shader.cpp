#include "Shader.h"
#include "glm/gtc/type_ptr.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
    if(!loadFromFile(vertexPath, fragmentPath, geometryPath))
        throw std::runtime_error("Shader init failed!");
}

Shader::~Shader()
{
    glDeleteProgram(ID);
}

void Shader::use()
{
    glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value)
{
    glUniform1i(getUniformLocation(name), (int)value); 
}

void Shader::setInt(const std::string &name, int value)
{ 
    glUniform1i(getUniformLocation(name), value); 
}

void Shader::setFloat(const std::string &name, float value)
{ 
    glUniform1f(getUniformLocation(name), value); 
}

void Shader::setVec2(const std::string &name, float x, float y)
{
    glUniform2f(getUniformLocation(name), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value)
{
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

void Shader::setVec3(const std::string &name, float x, float y, float z)
{
    glUniform3f(getUniformLocation(name), x, y, z); 
}

void Shader::setMat3(const std::string &name, glm::mat3 mat)
{
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setMat4(const std::string &name, glm::mat4 mat)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(mat));
}

bool Shader::loadFromFile(const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if(!geometryPath.empty())
        {
            gShaderFile.open(geometryPath);
            std::stringstream gShaderStream;
            gShaderStream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch(std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ:" << vertexPath << std::endl;
        return false;
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, NULL);
    glCompileShader(vertexShader);
    if(!checkError(vertexShader, "VERTEX", vertexPath)) return false;

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
    glCompileShader(fragmentShader);
    if(!checkError(fragmentShader, "FRAGMENT", fragmentPath)) return false;

    unsigned int geometryShader;
    if(!geometryPath.empty())
    {
        const char * gShaderCode = geometryCode.c_str();
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &gShaderCode, NULL);
        glCompileShader(geometryShader);
        if(!checkError(geometryShader, "GEOMETRY", geometryPath)) return false;
    }

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    if(!geometryPath.empty())
        glAttachShader(ID, geometryShader);
    glLinkProgram(ID);
    if(!checkError(ID, "PROGRAM", vertexPath)) return false;

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(!geometryPath.empty())
        glDeleteShader(geometryShader);
    return true;
}

bool Shader::checkError(GLuint shader, std::string type, const std::string& path)
{
    int  success;
    char infoLog[512];
    if(type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << path << " " << type <<"::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success)
        {
            glGetProgramInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
            return false;
        }
    }
    return true;
}

GLint Shader::getUniformLocation(const std::string &name)
{
    // 缓存优化：避免重复查询
    if(m_uniformCache.find(name) != m_uniformCache.end())
        return m_uniformCache[name];

    GLint location = glGetUniformLocation(ID, name.c_str());
    if(location == -1)
        std::cerr << "Warning: Uniform '" << name << "' not found!" << std::endl;
    m_uniformCache[name] = location;
    return location;
}
