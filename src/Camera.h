//
// Created by mikes on 11.07.2025.
//
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Map.h"

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {

public:
    glm::vec3 position;
    Camera(const Map& map, glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), 
           glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f), 
           float yaw = -90.0f, float pitch = 0.0f);
    
    glm::mat4 getViewMatrix() const;
    void processKeyboard(CameraMovement direction, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    
    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getFront() const { return front; }
    float getMovementSpeed() const { return movementSpeed; }
    float getMouseSensitivity() const { return mouseSensitivity; }
    
    // Setters
    void setMovementSpeed(float speed) { movementSpeed = speed; }
    void setMouseSensitivity(float sensitivity) { mouseSensitivity = sensitivity; }
    void setCollisionRadius(float radius) { collisionRadius = radius; }

private:
    const Map& map;

    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float movementSpeed;
    float mouseSensitivity;


    float collisionRadius = 0.3f;
    float playerHeight = 1.8f;
    void updateCameraVectors();


    bool checkCollision(const glm::vec3& newPosition) const;
    bool isWallAt(int x, int z) const;
    bool checkCubeCollision(const glm::vec3& playerPos, const glm::vec3& cubePos) const;
    glm::vec3 handleCollision(const glm::vec3& oldPos, const glm::vec3& newPos) const;

};