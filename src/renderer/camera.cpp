#include "camera.h"

Camera::Camera(float width, float height, float movementSpeed, float mouseSpeed, float fov)
    :m_width(width), m_height(height), pos(0.0f, 2.0f, 5.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed), m_fov(fov), Foucs(true)
{
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouse = true;
    m_aspect = width / height;
    m_yaw = glm::degrees(asinf(front.z / sqrtf(front.x * front.x + front.z + front.z)));
    m_pitch = glm::degrees(asinf(front.y / sqrtf(front.x * front.x + front.z + front.z)));
    calView();
    calProjection();
}

Camera::Camera(float width, float height, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float movementSpeed, float mouseSpeed, float fov)
    :m_width(width), m_height(height), pos(pos), front(front), up(up), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed), m_fov(fov), Foucs(true)
{
    lastX = width / 2.0f;
    lastY = height / 2.0f;
    firstMouse = true;
    m_aspect = width / height;
    m_yaw = glm::degrees(asinf(front.z / sqrtf(front.x * front.x + front.z + front.z)));
    m_pitch = glm::degrees(asinf(front.y / sqrtf(front.x * front.x + front.z + front.z)));
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
    Foucs = false;
}

void Camera::calView()
{
    view = glm::lookAt(pos, pos + front, up);
}

void Camera::calProjection()
{
    projection = glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 100.0f);
}
