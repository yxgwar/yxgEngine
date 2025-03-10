#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <string>

class Object
{
public:
    Object(const char* name);
    virtual ~Object() = default;

    virtual void SetPosition(glm::vec3 pos);
    virtual void SetRotation(float radius, glm::vec3 direction);
    virtual void SetScale(glm::vec3 scale);

    inline glm::mat4 GetModelMatrix() {return m_modelM;}
    inline glm::mat3 GetNormalMatrix() {return m_NormalM;}
    inline glm::vec3 GetPosition() {return m_position;}
protected:
    void calcuModel();
    glm::vec3 m_position = glm::vec3(0.0f);
    glm::quat m_rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 m_scale = glm::vec3(1.0f);
    glm::mat4 m_modelM;
    glm::mat3 m_NormalM;
    std::string m_name;
};