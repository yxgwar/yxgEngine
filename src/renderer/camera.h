#pragma once

#include "glm/glm.hpp"
#include "window.h"

class Camera
{
public:
    Camera(float width, float height, float movementSpeed = 5.0f, float mouseSpeed = 0.1f, float fov = 45.0f);
    Camera(float width, float height, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float movementSpeed = 5.0f, float mouseSpeed = 0.1f, float fov = 45.0f);

    inline glm::mat4 getView() const {return view;}
    inline glm::mat4 getProjection() const {return projection;}
    inline glm::vec3 getPosition() const {return pos;}
    inline glm::vec3 getFront() const {return front;}

    void OnUpdate(float deltaTime, Window& window);

private:
    void calView();
    void calProjection();
private:
    bool m_foucs;
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 projection;

    float movementSpeed;
    float mouseSpeed;

    float m_yaw;
    float m_pitch;

    float m_fov;
    float m_width;
    float m_height;
    float m_aspect;
};