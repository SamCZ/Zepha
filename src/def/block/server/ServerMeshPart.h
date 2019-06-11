//
// Created by aurailus on 10/06/19.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef ZEUS_SERVERMESHPART_H
#define ZEUS_SERVERMESHPART_H

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <vec4.hpp>
#include <gtx/normal.hpp>

#include "../MeshMod.h"
#include "../../../game/scene/world/graph/MeshVertex.h"
#include "../../texture/TextureAtlas.h"

struct ServerMeshPart {
    ServerMeshPart(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::string texture);

    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;

    std::string texture;

    MeshMod meshMod;
    float modValue;
};



#endif //ZEUS_SERVERMESHPART_H

#pragma clang diagnostic pop