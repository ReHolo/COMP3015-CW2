#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <cmath>

Camera::Camera()
    : Position(glm::vec3(0.0f, 0.0f, 3.0f)),
    up(glm::vec3(0.0f, 1.0f, 0.0f)),
    window(nullptr),
    yaw(-90.0f),           // 默认朝向 -Z 轴
    pitch(0.0f),
    lastX(0.0f),
    lastY(0.0f),
    firstMouse(true),
    mouseSensitivity(0.1f)
{
    // 根据 yaw 和 pitch 计算初始前方向，并设置 target
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    target = Position + glm::normalize(front);
}

Camera::Camera(const glm::vec3& pos, const glm::vec3& tgt, const glm::vec3& up)
    : Position(pos),
    target(tgt),
    up(up),
    window(nullptr),
    yaw(-90.0f),
    pitch(0.0f),
    lastX(0.0f),
    lastY(0.0f),
    firstMouse(true),
    mouseSensitivity(0.1f)
{
    // 可以根据 (tgt - pos) 计算 yaw 和 pitch，此处使用默认值
}

void Camera::setPosition(const glm::vec3& pos) {
    Position = pos;
}

void Camera::setTarget(const glm::vec3& t) {
    target = t;
}

void Camera::setUp(const glm::vec3& u) {
    up = u;
}

glm::mat4 Camera::getViewMatrix() {
    // 根据当前 yaw 和 pitch 计算前方向
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    target = Position + glm::normalize(front);
    return glm::lookAt(Position, target, up);
}

void Camera::setWindow(GLFWwindow* win) {
    window = win;
}

void Camera::processInput(float deltaTime) {
    if (window == nullptr)
        return; // 未设置窗口则不处理输入

    float cameraSpeed = 20.0f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        moveForward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        moveBackward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        moveLeft(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        moveRight(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        moveUp(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        moveDown(cameraSpeed);
}

void Camera::processMouseMovement(double xpos, double ypos) {
    if (firstMouse) {
        lastX = static_cast<float>(xpos);
        lastY = static_cast<float>(ypos);
        firstMouse = false;
    }

    float xoffset = static_cast<float>(xpos) - lastX;
    float yoffset = lastY - static_cast<float>(ypos); 
    lastX = static_cast<float>(xpos);
    lastY = static_cast<float>(ypos);

    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    
}

void Camera::moveForward(float delta) {
    glm::vec3 front = glm::normalize(target - Position);
    Position += front * delta;
    target += front * delta;
}

void Camera::moveBackward(float delta) {
    glm::vec3 front = glm::normalize(target - Position);
    Position -= front * delta;
    target -= front * delta;
}

void Camera::moveLeft(float delta) {
    glm::vec3 front = glm::normalize(target - Position);
    glm::vec3 right = glm::normalize(glm::cross(front, up));
    Position -= right * delta;
    target -= right * delta;
}

void Camera::moveRight(float delta) {
    glm::vec3 front = glm::normalize(target - Position);
    glm::vec3 right = glm::normalize(glm::cross(front, up));
    Position += right * delta;
    target += right * delta;
}

void Camera::moveUp(float delta) {
    Position += up * delta;
    target += up * delta;
}

void Camera::moveDown(float delta) {
    Position -= up * delta;
    target -= up * delta;
}
