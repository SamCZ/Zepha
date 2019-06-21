//
// Created by aurailus on 29/11/18.
//

#include "Texture.h"

Texture::Texture(std::string file) {
    fileLocation = std::move(file);
    loadFromFile(this->fileLocation);
}

void Texture::loadFromFile(std::string file) {
    unsigned char *texData = stbi_load(file.c_str(), &width, &height, &bitDepth, 0);

    if (!texData) {
        printf("Failed to find texture at '%s'\n", file.c_str());
        return;
    }

    loadFromBytes(texData, width, height);

    stbi_image_free(texData);
}

void Texture::loadFromBytes(unsigned char* bytes, int width, int height, GLint interpolation, GLint repeatMode) {
    if (textureID != 0) clear();

    this->width = width;
    this->height = height;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, repeatMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, repeatMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpolation);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpolation);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Unbind current texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::updateTexture(int x, int y, int width, int height, unsigned char *bytes) {
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
}


void Texture::enable(GLuint position) {
    glActiveTexture(GL_TEXTURE0 + position);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::clear() {
    glDeleteTextures(1, &textureID);
    textureID = 0;
    fileLocation = "";

    width = 0;
    height = 0;
    bitDepth = 0;
}

Texture::~Texture() {
    clear();
}