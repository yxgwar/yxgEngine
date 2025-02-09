#pragma once

#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

enum class CameraKeyCode
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

class Camera
{
public:
    Camera(float width, float height, float movementSpeed = 5.0f, float mouseSpeed = 0.1f, float fov = 45.0f);
    Camera(float width, float height, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float movementSpeed = 5.0f, float mouseSpeed = 0.1f, float fov = 45.0f);

    inline glm::mat4 getView() const {return view;}
    inline glm::mat4 getProjection() const {return projection;}
    inline glm::vec3 getPosition() const {return pos;}
    inline glm::vec3 getFront() const {return front;}

    void KeyboardControl(CameraKeyCode code, float deltaTime);
    void MouseControl(float xPos, float yPos);
    void ScrollControl(float yOffset);

    void FreeCamera();
private:
    void calView();
    void calProjection();
private:
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

    float lastX;
    float lastY;
    bool firstMouse;
};