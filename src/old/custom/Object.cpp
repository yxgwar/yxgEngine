#include "Object.h"

Object::Object(const char *name)
    :m_modelM(glm::mat4(1.0f)), m_NormalM(glm::mat3(1.0f)), m_name(name)
{

}

void Object::SetPosition(glm::vec3 pos)
{
    m_position = pos;
    calcuModel();
}

void Object::SetRotation(float radius, glm::vec3 direction)
{
    m_rotation = glm::angleAxis(glm::radians(radius), glm::normalize(direction));
    calcuModel();
}

void Object::SetScale(glm::vec3 scale)
{
    m_scale = scale;
    calcuModel();
}

void Object::calcuModel()
{
    m_modelM = glm::translate(glm::mat4(1.0f), m_position);
    m_modelM *= glm::mat4_cast(m_rotation);
    m_modelM = glm::scale(m_modelM, m_scale);
    m_NormalM = glm::transpose(glm::inverse(glm::mat3(m_modelM)));
}
