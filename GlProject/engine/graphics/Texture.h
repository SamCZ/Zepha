//

// Created by aurailus on 29/11/18.
//

#ifndef GLPROJECT_TEXTURE_H
#define GLPROJECT_TEXTURE_H

#include <glew.h>
#include <stb_image.h>

class Texture {
public:
    Texture();
    Texture(char* fileLocation);

    void load();
    void use();
    void clear();

    ~Texture();

private:
    GLuint textureID;
    int width, height, bitDepth;

    const char* fileLocation;
};


#endif //GLPROJECT_TEXTURE_H
