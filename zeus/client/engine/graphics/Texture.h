//
// Created by aurailus on 29/11/18.
//

#ifndef GLPROJECT_TEXTURE_H
#define GLPROJECT_TEXTURE_H

#include <glew.h>
#include <iostream>
#include <stb_image.h>

class Texture {
public:
    Texture();

    explicit Texture(char* fileLocation);

    void load();
    void load(unsigned char* bytes, int width, int height);
    void use();
    void clear();

    ~Texture();

private:
    GLuint textureID;
    int width, height, bitDepth;

    const char* fileLocation;
};


#endif //GLPROJECT_TEXTURE_H
