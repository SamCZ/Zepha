//
// Created by aurailus on 02/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef GLPROJECT_MESHPART_H
#define GLPROJECT_MESHPART_H

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <vec4.hpp>
#include <gtx/normal.hpp>

#include "MeshMod.h"
#include "TextureAtlas.h"
#include "../../client/graphics/mesh/MeshVertex.h"

class MeshVertexIter;
class MeshIndexIter;

struct MeshPart {
    MeshPart(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::string texture, TextureAtlas* atlas);

    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;

    MeshMod meshMod;
    float modValue;
};


#endif //GLPROJECT_MESHPART_H

#pragma clang diagnostic pop