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
    Camera(float movementSpeed = 2.5f, float mouseSpeed = 0.1f);
    Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float movementSpeed = 2.5f, float mouseSpeed = 0.1f);

    inline glm::mat4 getView() {return view;}

    void KeyboardControl(CameraKeyCode code, float deltaTime);
    void MouseControl(float xOffset, float yOffset);
private:
    void calView();
private:
    glm::vec3 pos;
    glm::vec3 front;
    glm::vec3 up;

    glm::mat4 view;
    glm::mat4 projection;

    float movementSpeed;
    float mouseSpeed;

    float yaw;
    float pitch;
};