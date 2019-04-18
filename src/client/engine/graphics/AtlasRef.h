//
// Created by aurailus on 17/04/19.
//

#ifndef ZEUS_ATLASREF_H
#define ZEUS_ATLASREF_H


#include <vec4.hpp>
#include <string>

struct AtlasRef {
    int tileX;
    int tileY;
    int tileWidth;
    int tileHeight;

    glm::vec4 uv;
    std::string name;

    int width;
    int height;
};


#endif //ZEUS_ATLASREF_H
