//
// Created by mikes on 10.07.2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H
#include <glad/glad.h>


class Texture
{
public:


            Texture(const char* fileName, GLenum textureType);
            ~Texture();
    void    Bind(GLenum texture) const;
    void    Unbind() const;
private:
    unsigned int textureID;
};



#endif //TEXTURE_H
