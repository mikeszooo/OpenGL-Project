//
// Created by mikes on 13.07.2025.
//

#include "Map.h"


Map::Map(std::vector<std::vector<int>> map, int width, int height){
    this->map = map;
    this->width = width;
    this->height = height;
};
void Map::InitBuffers()
{
    float vertices[] = {
        //  ŚCIANA -Z
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

        //  ŚCIANA +Z  ← tez “0,0→1,1”!
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,

        //  ŚCIANA -X
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,

        //  ŚCIANA +X
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 0.0f,

        //  ŚCIANA -Y
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,

        //  ŚCIANA +Y
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 0.0f
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glEnable(GL_DEPTH_TEST);
    glBindVertexArray(0);
}
void Map::RenderMap()
{

    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            if (map[i][j] == 1) cubePositions.push_back(glm::vec3(j, 0, i));
            //if (map[i][j] == 0) cubePositions.push_back(glm::vec3(j, -1, i));
        }

    }

}
void Map::Draw(Shader& shader) {
    shader.use();
    glBindVertexArray(VAO);
    for (auto& pos : cubePositions) {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
        shader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
    glBindVertexArray(0);
}
void Map::DeleteBuffers()
{
    glDeleteVertexArrays(1,&VAO);
    glDeleteBuffers(1, &VBO);
}
bool Map::Raycast(const glm::vec3& start, const glm::vec3& end, float& outHitDist) const {
    // Rzutujemy na 2D (x,z)
    glm::vec2 s{ start.x, start.z };
    glm::vec2 e{ end.x,   end.z   };
    glm::vec2 delta = e - s;
    float maxDist = glm::length(delta);
    glm::vec2 dir = delta / maxDist;

    // Parametry kroku
    const float step = 0.1f;             // co 0.1 jednostki
    int steps = int(maxDist / step);

    for (int i = 1; i <= steps; ++i) {
        glm::vec2 p = s + dir * (i * step);
        int mx = int(floor(p.x));
        int mz = int(floor(p.y));
        if (mx < 0 || mz < 0 || mx >= width || mz >= height) break;
        if (map[mz][mx] != 0) {
            outHitDist = i * step;
            return true;  // napotkaliśmy ścianę
        }
    }
    return false;  // nic nie zasłoniło
}
