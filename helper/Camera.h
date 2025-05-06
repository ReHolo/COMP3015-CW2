#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>


class Camera {
public:
    Camera();
    Camera(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

    
    void setPosition(const glm::vec3& pos);
    void setTarget(const glm::vec3& target);
    void setUp(const glm::vec3& up);

    
    glm::mat4 getViewMatrix();

    
    void setWindow(GLFWwindow* window);

    void processInput(float deltaTime);
    void processMouseMovement(double xpos, double ypos);

   
    void moveForward(float delta);
    void moveBackward(float delta);
    void moveLeft(float delta);
    void moveRight(float delta);
    void moveUp(float delta);
    void moveDown(float delta);

public:
    glm::vec3 Position;
    glm::vec3 target; 
    glm::vec3 up;
    GLFWwindow* window; 

    glm::mat4 Projection;
    
    float yaw;
    float pitch;
    float lastX;
    float lastY;
    bool firstMouse;
    float mouseSensitivity;
};

#endif // CAMERA_H
