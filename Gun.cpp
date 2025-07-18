//
// Created by mikes on 18.07.2025.
//

#include "Gun.h"

#include "glm/ext/matrix_transform.hpp"


Gun::Gun(const char *filename, glm::vec3 pos,const unsigned int SCR_WIDTH, const unsigned int SCR_HEIGHT):
    pos(pos),
    texture(filename, GL_RGBA),
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
    float crosshairVerts[] = {
        // pozioma linia (długość 20 pikseli)
        -10.0f,  0.0f,
         10.0f,  0.0f,
         // pionowa linia
         0.0f, -10.0f,
         0.0f,  10.0f
     };

    glGenVertexArrays(1, &chVAO);
    glGenBuffers(1, &chVBO);

    glBindVertexArray(chVAO);
    glBindBuffer(GL_ARRAY_BUFFER, chVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVerts), crosshairVerts, GL_STATIC_DRAW);

    // tylko pozycja 2D
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
void Gun::RenderCrosshair(Shader &shader)
{

    // wyłącz głębię (choć gun.Render już to robi i przywraca)
    glDisable(GL_DEPTH_TEST);

    // ustaw HUD‑shader i orto‐projekcję
    shader.use();
    glm::mat4 ortho = glm::ortho(
        0.0f, (float)SCR_WIDTH,
        0.0f, (float)SCR_HEIGHT
    );
    shader.setMat4("projection", ortho);

    // model = przesunięcie do środka ekranu
    glm::mat4 model = glm::translate(
        glm::mat4(1.0f),
        glm::vec3(SCR_WIDTH * 0.5f, SCR_HEIGHT * 0.5f, 0.0f)
    );
    shader.setMat4("model", model);

    // narysuj 4 wierzchołki liniami
    glBindVertexArray(chVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    // przywróć test głębokości
    glEnable(GL_DEPTH_TEST);
}

