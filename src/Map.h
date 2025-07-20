//
// Created by mikes on 13.07.2025.
//

#ifndef MAP_H
#define MAP_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Shader.h"


class Map
{
public:
    Map(std::vector<std::vector<int>> map, int width, int height);
    void RenderMap();
    void InitBuffers();
    void DeleteBuffers();
    void Draw(Shader& shader);
    bool isWallAt(float x, float z) const;
    bool isCollidingSimple(float x, float z, float size) const;
    std::vector<std::vector<int>> map;
    std::vector<glm::vec3> cubePositions;
    int width;
    int height;
    unsigned int VBO,VAO;
    bool Raycast(const glm::vec3& start, const glm::vec3& end, float& outHitDist) const;
private:


};



#endif //MAP_H
