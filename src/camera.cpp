#include "camera.h"

Camera::Camera(float width, float height, float movementSpeed, float mouseSpeed, float fov)
    :m_width(width), m_height(height), pos(0.0f, 0.0f, 3.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed), m_yaw(-90.0f), m_pitch(0.0f), m_fov(fov)
{
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouse = true;
    m_aspect = width / height;
    calView();
    calProjection();
}

Camera::Camera(float width, float height, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float movementSpeed, float mouseSpeed, float fov)
    :m_width(width), m_height(height), pos(pos), front(front), up(up), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed), m_yaw(-90.0f), m_pitch(0.0f), m_fov(fov)
{
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouse = true;
    m_aspect = width / height;
    calView();
    calProjection();
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
        case CameraKeyCode::UP:
        {
            pos += up * cameraSpeed;
            break;
        }
        case CameraKeyCode::DOWN:
        {
            pos -= up * cameraSpeed;
            break;
        }
    }
    calView();
}

void Camera::MouseControl(float xPos, float yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    m_yaw += xOffset * mouseSpeed;
    m_pitch += yOffset * mouseSpeed;

    if(m_pitch > 89.0f) m_pitch = 89.0f;
    if(m_pitch < -89.0f) m_pitch = -89.0f;

    front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
    front.y = sin(glm::radians(m_pitch));
    front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
    front = glm::normalize(front);

    calView();
}

void Camera::ScrollControl(float yOffset)
{
    if(m_fov >= 1.0f && m_fov <= 45.0f)
        m_fov -= yOffset;
    if(m_fov <= 1.0f)
        m_fov = 1.0f;
    if(m_fov >= 45.0f)
        m_fov = 45.0f;
    calProjection();
}

void Camera::FreeCamera()
{
    firstMouse = true;
}

void Camera::calView()
{
    view = glm::lookAt(pos, pos + front, up);
}

void Camera::calProjection()
{
    projection = glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 100.0f);
}
