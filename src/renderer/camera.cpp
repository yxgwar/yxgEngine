#include "camera.h"

Camera::Camera(float width, float height, float movementSpeed, float mouseSpeed, float fov)
    :m_width(width), m_height(height), pos(0.0f, 2.0f, 5.0f), front(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed), m_fov(fov), m_foucs(false)
{
    m_aspect = width / height;
    m_yaw = glm::degrees(asinf(front.z / sqrtf(front.x * front.x + front.z * front.z)));
    m_pitch = glm::degrees(asinf(front.y / sqrtf(front.x * front.x + front.z * front.z)));
    calView();
    calProjection();
}

Camera::Camera(float width, float height, glm::vec3 pos, glm::vec3 front, glm::vec3 up, float movementSpeed, float mouseSpeed, float fov)
    :m_width(width), m_height(height), pos(pos), front(front), up(up), movementSpeed(movementSpeed), mouseSpeed(mouseSpeed), m_fov(fov), m_foucs(false)
{
    m_aspect = width / height;
    m_yaw = glm::degrees(asinf(front.z / sqrtf(front.x * front.x + front.z * front.z)));
    m_pitch = glm::degrees(asinf(front.y / sqrtf(front.x * front.x + front.z * front.z)));
    calView();
    calProjection();
}

void Camera::OnUpdate(float deltaTime, Window& window)
{
    if(Input::GetKeyDown(GLFW_KEY_ESCAPE))
    {
        if(m_foucs)
        {
            window.UnFocus();
            m_foucs = false;
        }
        else
        {
            window.Focus();
            m_foucs = true;
        }
    }
    if(m_foucs)
    {
        float cameraSpeed = movementSpeed* deltaTime;
        if(Input::GetKey(GLFW_KEY_W))
            pos += cameraSpeed * front;
        if(Input::GetKey(GLFW_KEY_S))
            pos -= cameraSpeed * front;
        if(Input::GetKey(GLFW_KEY_A))
            pos -= glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if(Input::GetKey(GLFW_KEY_D))
            pos += glm::normalize(glm::cross(front, up)) * cameraSpeed;
        if(Input::GetKey(GLFW_KEY_SPACE))
            pos += up * cameraSpeed;
        if(Input::GetKey(GLFW_KEY_LEFT_SHIFT))
            pos -= up * cameraSpeed;

        const Input::MouseState& mouse = Input::GetMouseState();
        m_yaw += mouse.deltaX * mouseSpeed;
        m_pitch -= mouse.deltaY * mouseSpeed;
        m_pitch = glm::clamp(m_pitch, -89.0f, 89.0f);

        front.x = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));
        front.y = sin(glm::radians(m_pitch));
        front.z = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
        front = glm::normalize(front);

        calView();
    }
}

void Camera::calView()
{
    view = glm::lookAt(pos, pos + front, up);
}

void Camera::calProjection()
{
    projection = glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 100.0f);
}
