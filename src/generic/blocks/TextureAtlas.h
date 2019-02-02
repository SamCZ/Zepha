//
// Created by aurailus on 06/12/18.
//

#ifndef GLPROJECT_TEXTUREATLAS_H
#define GLPROJECT_TEXTUREATLAS_H

#include <glew.h>
#include <iostream>
#include <vec4.hpp>
#include <list>
#include <map>

#include "../../client/engine/graphics/Texture.h"

class TextureAtlas {
public:
    TextureAtlas();
    explicit TextureAtlas(const char* directory);

    Texture* getTexture();
    glm::vec4* getUVs(std::string* texture);
private:
    GLint maxTexSize;

    Texture* texture;
    std::map<std::string, glm::vec4> textures;
};


#endif //GLPROJECT_TEXTUREATLAS_H
