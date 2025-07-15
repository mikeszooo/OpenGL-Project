//
// Created by mikes on 11.07.2025.
//
#include "Camera.h"
#include <algorithm>


Camera::Camera(const Map& map, glm::vec3 position, glm::vec3 worldUp, float yaw, float pitch)
    : map(map),
      position(position),
      worldUp(worldUp),
      yaw(yaw),
      pitch(pitch),
      front(glm::vec3(0.0f, 0.0f, -1.0f)),
      movementSpeed(2.5f),
      mouseSensitivity(0.1f)
{
    updateCameraVectors();
}



glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    glm::vec3 newPosition = position;

    if (direction == FORWARD)
        newPosition += front * velocity;
    if (direction == BACKWARD)
        newPosition -= front * velocity;
    if (direction == LEFT)
        newPosition -= right * velocity;
    if (direction == RIGHT)
        newPosition += right * velocity;


    newPosition = handleCollision(position, newPosition);
    position = newPosition;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    updateCameraVectors();
}



void Camera::updateCameraVectors() {
    glm::vec3 frontVec;
    frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    frontVec.y = sin(glm::radians(pitch));
    frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(frontVec);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}

bool Camera::checkCubeCollision(const glm::vec3& playerPos, const glm::vec3& cubePos) const {

    float cubeSize = 1.0f;
    float halfCube = cubeSize / 2.0f;


    float cubeMinX = cubePos.x - halfCube;
    float cubeMaxX = cubePos.x + halfCube;
    float cubeMinY = cubePos.y - halfCube;
    float cubeMaxY = cubePos.y + halfCube;
    float cubeMinZ = cubePos.z - halfCube;
    float cubeMaxZ = cubePos.z + halfCube;


    float playerMinX = playerPos.x - collisionRadius;
    float playerMaxX = playerPos.x + collisionRadius;
    float playerMinY = playerPos.y - playerHeight / 2.0f;
    float playerMaxY = playerPos.y + playerHeight / 2.0f;
    float playerMinZ = playerPos.z - collisionRadius;
    float playerMaxZ = playerPos.z + collisionRadius;

    // AABB collision
    return (playerMinX < cubeMaxX && playerMaxX > cubeMinX &&
            playerMinY < cubeMaxY && playerMaxY > cubeMinY &&
            playerMinZ < cubeMaxZ && playerMaxZ > cubeMinZ);
}

bool Camera::checkCollision(const glm::vec3& newPosition) const {

    for (int x = 0; x < map.width; x++) {
        for (int z = 0; z < map.height; z++) {
            if (map.map[x][z] == 1) {
                glm::vec3 wallPos(x, 0, z);
                if (checkCubeCollision(newPosition, wallPos)) {
                    return true;
                }
            }
        }
    }

    return false;
}

bool Camera::isWallAt(int x, int z) const {

    if (x < 0 || x >= map.width || z < 0 || z >= map.height) {
        return true;
    }


    return map.map[x][z] == 1;
}

glm::vec3 Camera::handleCollision(const glm::vec3& oldPos, const glm::vec3& newPos) const {

    if (!checkCollision(newPos)) {
        return newPos;
    }


    glm::vec3 xOnlyMove = glm::vec3(newPos.x, oldPos.y, oldPos.z);
    if (!checkCollision(xOnlyMove)) {
        return xOnlyMove;
    }


    glm::vec3 zOnlyMove = glm::vec3(oldPos.x, oldPos.y, newPos.z);
    if (!checkCollision(zOnlyMove)) {
        return zOnlyMove;
    }


    return oldPos;
}