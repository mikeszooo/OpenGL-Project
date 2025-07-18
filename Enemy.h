//
// Created by mikes on 17.07.2025.
//

#ifndef ENEMY_H
#define ENEMY_H
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <iostream>
#include "Texture.h"
#include "Shader.h"
#include "glm/ext/matrix_transform.hpp"
#include "Map.h"


class Enemy
{
public:
    Enemy(glm::vec3 position, Shader& shader);
    void Render(glm::vec3& cameraPos, float deltaTime);
    void InitBuffers();
    bool CanSee(const glm::vec3& playerPos, Map& map) const;
    void Update(const glm::vec3& playerPos, Map& map, float deltaTime);
    void ShootAt(const glm::vec3& playerPos);
private:
    std::vector<Texture> textures;
    glm::vec3 position;
    Shader& shader;
    unsigned int VBO,VAO;
    float timeSinceLastShot;
    float shootInterval;
    float shotAnimTimer      = 0.0f;
    const float shotAnimDuration = 0.5f; // animacja trwa 0.5 s
    const int   shotFrames       = 3;   // Enemy1 = idle, Enemy2/3 = animacja
    bool        isAnimatingShot  = false;
};



#endif //ENEMY_H
