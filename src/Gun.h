//
// Created by mikes on 18.07.2025.
//

#ifndef GUN_H
#define GUN_H
#include "Texture.h"
#include "glm/vec3.hpp"
#include "glm/fwd.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "Shader.h"
#include "Enemy.h"

class Gun
{
public:
    Gun(const char* gun,const char* crosshair, glm::vec3 pos,const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);
    void InitBuffers();
    void Render(Shader& shader);
    void InitBuffersCrosshair();
    void RenderCrosshair(Shader& shader);
    void PlayAnimation();
    void GetRayDirection(glm::mat4 projectionMatrix, glm::mat4 viewMatrix,glm::vec3 cameraPos);
    bool CheckEnemyHit(const std::vector<Enemy>& enemies, Enemy*& hitEnemy, float maxDistance);
    glm::vec3 GetRayDir() const { return rayDir; }
private:
    glm::vec3 pos;
    Texture texture;
    Texture crosshair;
    unsigned int gunVAO, gunVBO, gunEBO;
    const unsigned int SCR_WIDTH, SCR_HEIGHT;
    unsigned int chVAO, chVBO, chEBO;
    glm::vec3 rayDir, rayStart;
};



#endif //GUN_H
