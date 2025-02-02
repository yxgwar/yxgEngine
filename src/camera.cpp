#include "camera.h"

Camera::Camera(float movementSpeed, float mouseSpeed)
    :pos(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed), yaw(-90.0f), pitch(0.0f)
{
    calView();
}

Camera::Camera(glm::vec3 pos, glm::vec3 front, glm::vec3 up, float movementSpeed, float mouseSpeed)
    :pos(pos), front(front), up(up), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed), yaw(-90.0f), pitch(0.0f)
{
    calView();
}

void Camera::KeyboardControl(CameraKeyCode code, float deltaTime)
{
    float cameraSpeed = movementSpeed* deltaTime;
    switch(code)
    {
        case CameraKeyCode::FORWARD:
        {
            pos += cameraSpeed * front;
            break;
        }
        case CameraKeyCode::BACKWARD:
        {
            pos -= cameraSpeed * front;
            break;
        }
        case CameraKeyCode::LEFT:
        {
            pos -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
            break;
        }
        case CameraKeyCode::RIGHT:
        {
            pos += glm::normalize(glm::cross(front, up)) * cameraSpeed;
            break;
        }
    }
    calView();
}

void Camera::MouseControl(float xOffset, float yOffset)
{
    yaw += xOffset * mouseSpeed;
    pitch += yOffset * mouseSpeed;

    if(pitch > 89.0f) pitch = 89.0f;
    if(pitch < -89.0f) pitch = -89.0f;

    front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    front.y = sin(glm::radians(pitch));
    front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    front = glm::normalize(front);

    calView();
}

void Camera::calView()
{
    view = glm::lookAt(pos, pos + front, up);
}


