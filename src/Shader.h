//
// Created by mikes on 08.07.2025.
//

#ifndef SHADER1_H
#define SHADER1_H

#include <glad/glad.h>

#include <string>
#include<fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>


class Shader
{
public:
    unsigned int ID;

    Shader(const char* vertexPath, const char* fragmentPath);

    void use();

    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
   void checkCompileErrors(unsigned int shader, std::string type);
};



#endif //SHADER1_H
