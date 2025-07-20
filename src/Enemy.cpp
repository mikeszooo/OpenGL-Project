//
// Created by mikes on 17.07.2025.
//

#include "Enemy.h"

#include "Shader.h"


Enemy::Enemy(glm::vec3 position, Shader& shader):

position(position),
shader(shader),
timeSinceLastShot(0.0f),
shootInterval(2.0f)
{
    textures.reserve(3);
    textures.emplace_back(R"(D:\PROJEKTINZ\Textures\Enemy\Enemy1.png)", GL_RGBA);
    textures.emplace_back(R"(D:\PROJEKTINZ\Textures\Enemy\Enemy2.png)", GL_RGBA);
    textures.emplace_back(R"(D:\PROJEKTINZ\Textures\Enemy\Enemy3.png)", GL_RGBA);


}
void Enemy::InitBuffers()
{
    float quadVertices[] = {
        // pos      // tex coords
        -0.25f, -0.25f,  0.0f, 0.0f,
         0.25f, -0.25f,  1.0f, 0.0f,
         0.25f,  0.35f,  1.0f, 1.0f,

         0.25f,  0.35f,  1.0f, 1.0f,
        -0.25f,  0.35f,  0.0f, 1.0f,
        -0.25f, -0.25f,  0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0); // position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float))); // texCoords
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}
void Enemy::Render(glm::vec3& cameraPos, float deltaTime){


    shader.use();
    glBindVertexArray(VAO);

    int texIndex = 0;
    if (isAnimatingShot) {
        float frameTime = shotAnimDuration / (shotFrames - 1);
        int   frame     = static_cast<int>(shotAnimTimer / frameTime);
        if (frame >= shotFrames - 1) frame = shotFrames - 2;
        texIndex = 1 + frame; // textures[1] i textures[2]
    }
    textures[texIndex].Bind(GL_TEXTURE0);


    glm::vec3 toCam = glm::normalize(cameraPos - position);
    float angle = atan2(toCam.x, toCam.z);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));

    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);



}
bool Enemy::CanSee(const glm::vec3& playerPos, Map& map) const {
    float hitDist;

    if ( map.Raycast(position, playerPos, hitDist) ) {
        float playerDist = glm::length(playerPos - position);
        return hitDist >= playerDist;
    }

    return true;
}
void Enemy::Update(const glm::vec3& playerPos, Map& map, float deltaTime) {

    timeSinceLastShot += deltaTime;

    if (!isAnimatingShot && timeSinceLastShot >= shootInterval && CanSee(playerPos, map)) {
        ShootAt(playerPos);
        timeSinceLastShot = 0.0f;
        isAnimatingShot  = true;
        shotAnimTimer    = 0.0f;
    }

    if (isAnimatingShot) {
        shotAnimTimer += deltaTime;
        if (shotAnimTimer >= shotAnimDuration) {
            isAnimatingShot = false;
        }
    }
}
void Enemy::ShootAt(const glm::vec3& playerPos) {



    std::cout << "Shooting at player at position: " << playerPos.x << ", " << playerPos.y << ", " << playerPos.z << std::endl;

}



