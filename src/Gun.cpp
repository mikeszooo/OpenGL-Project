//
// Created by mikes on 18.07.2025.
//

#include "Gun.h"

#include "glm/ext/matrix_transform.hpp"


Gun::Gun(const char *gun, const char *crosshair,glm::vec3 pos,const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT):
    pos(pos),
    texture(gun, GL_RGBA),
    crosshair(crosshair,GL_RGBA),
    SCR_WIDTH(SCR_WIDTH),
    SCR_HEIGHT(SCR_HEIGHT)
{


}
void Gun::InitBuffers()
{
    float quadVertices[] = {
        //  pos.x, pos.y,   tex.u, tex.v
        0.0f,   0.0f,     0.0f, 1.0f,
      300.0f,   0.0f,     1.0f, 1.0f,
      300.0f, 300.0f,     1.0f, 0.0f,
        0.0f, 300.0f,     0.0f, 0.0f,
  };
    unsigned int quadIndices[] = { 0,1,2, 2,3,0 };

    glGenVertexArrays(1, &gunVAO);
    glGenBuffers(1, &gunVBO);
    glGenBuffers(1, &gunEBO);

    glBindVertexArray(gunVAO);

    glBindBuffer(GL_ARRAY_BUFFER, gunVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gunEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    // aPos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // aTex
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
void Gun::Render(Shader& shader) {

    glDisable(GL_DEPTH_TEST);

    // Ortograficzna projekcja w pikselach ekranu
    glm::mat4 projection = glm::ortho(
        0.0f, static_cast<float>(SCR_WIDTH),
        0.0f, static_cast<float>(SCR_HEIGHT)
    );

    shader.use();
    shader.setMat4("projection", projection);

    // Model: przesunięcie do dolnego środka
    glm::mat4 model = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(
            SCR_WIDTH * 0.5f - 150.0f,
            0.0f,
            0.0f
        )
    );
    shader.setMat4("model", model);

    // Rysuj quad
    glActiveTexture(GL_TEXTURE0);
    texture.Bind(GL_TEXTURE0);
    shader.setInt("gunTex", 0);

    glBindVertexArray(gunVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Przywróć depth test
    glEnable(GL_DEPTH_TEST);
}
void Gun::InitBuffersCrosshair()
{
    float quadVertices[] = {
        //  pos.x, pos.y,   tex.u, tex.v
        0.0f,   0.0f,     0.0f, 1.0f,
        30.0f,   0.0f,     1.0f, 1.0f,
        30.0f, 30.0f,     1.0f, 0.0f,
        0.0f, 30.0f,     0.0f, 0.0f,
  };
    unsigned int quadIndices[] = { 0,1,2, 2,3,0 };

    glGenVertexArrays(1, &chVAO);
    glGenBuffers(1, &chVBO);
    glGenBuffers(1, &chEBO);

    glBindVertexArray(chVAO);

    glBindBuffer(GL_ARRAY_BUFFER, chVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    // aPos
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // aTex
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}
void Gun::RenderCrosshair(Shader &shader)
{

    glDisable(GL_DEPTH_TEST);

    // Ortograficzna projekcja w pikselach ekranu
    glm::mat4 projection = glm::ortho(
        0.0f, static_cast<float>(SCR_WIDTH),
        0.0f, static_cast<float>(SCR_HEIGHT)
    );
    shader.use();
    shader.setMat4("projection", projection);

    // rozmiar quad’a (ten sam co w InitBuffersCrosshair)
    const float quadW = 30.0f;
    const float quadH = 30.0f;

    // policz środek ekranu minus połowa quad’a
    float x = SCR_WIDTH  * 0.5f - quadW * 0.5f;
    float y = SCR_HEIGHT * 0.5f - quadH * 0.5f;

    glm::mat4 model = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(x, y, 0.0f)
    );
    shader.setMat4("model", model);

    // Rysuj quad
    glActiveTexture(GL_TEXTURE0);
    crosshair.Bind(GL_TEXTURE0);
    shader.setInt("crossTex", 0);

    glBindVertexArray(chVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);
}
void Gun::GetRayDirection(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::vec3 cameraPos)
{
    float mouseX = static_cast<float>(SCR_WIDTH) *0.5f;
    float mouseY = static_cast<float>(SCR_HEIGHT) *0.5f;

    float x_ndc = 2.0f * (mouseX / (float)SCR_WIDTH)  - 1.0f;
    float y_ndc = 1.0f - 2.0f * (mouseY / (float)SCR_HEIGHT);

    glm::vec4 rayClip = glm::vec4(x_ndc, y_ndc, -1.0f, 1.0f);

    glm::mat4 invProj = glm::inverse(projectionMatrix);
    glm::vec4 rayEye  = invProj * rayClip;
    rayEye.z = -1.0f; rayEye.w = 0.0f;

    glm::mat4 invView = glm::inverse(viewMatrix);
    glm::vec4 rayWorld4 = invView * rayEye;
    rayStart = cameraPos;
    rayDir = glm::normalize(glm::vec3(rayWorld4));
    //std::cout << rayDir.x << " " << rayDir.y << " " << rayDir.z << std::endl;

}
bool Gun::CheckEnemyHit(const std::vector<Enemy>& enemies, Enemy*& hitEnemy, float maxDistance)
{
    float closestDistance = maxDistance;
    hitEnemy = nullptr;

    for (const auto& enemy : enemies) {
        glm::vec3 enemyPos = enemy.GetPosition();
        float enemyRadius = 0.3f;

        glm::vec3 toEnemy = enemyPos - rayStart;
        float projLength = glm::dot(toEnemy, rayDir);


        if (projLength < 0) continue;


        glm::vec3 closestPoint = rayStart + rayDir * projLength;
        float distance = glm::length(enemyPos - closestPoint);


        if (distance <= enemyRadius && projLength < closestDistance) {
            closestDistance = projLength;
            hitEnemy = const_cast<Enemy*>(&enemy);
        }
    }

    return hitEnemy != nullptr;
}


