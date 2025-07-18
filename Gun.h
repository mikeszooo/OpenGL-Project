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

class Gun
{
public:
    Gun(const char* filename, glm::vec3 pos,const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT);
    void InitBuffers();
    void Render(Shader& shader);
    void InitBuffersCrosshair();
    void RenderCrosshair(Shader& shader);
private:
    glm::vec3 pos;
    Texture texture;
    unsigned int gunVAO, gunVBO, gunEBO;
    const unsigned int SCR_WIDTH, SCR_HEIGHT;
    unsigned int chVAO, chVBO;
};



#endif //GUN_H
