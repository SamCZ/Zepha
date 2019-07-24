//
// Created by aurailus on 10/06/19.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef ZEUS_SERVERMESHPART_H
#define ZEUS_SERVERMESHPART_H

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <glm/vec4.hpp>
#include <glm/gtx/normal.hpp>

#include "../ShaderMod.h"
#include "../../../game/scene/world/graph/MeshVertex.h"
#include "../../texture/TextureAtlas.h"

struct ServerMeshPart {
    ServerMeshPart(const std::vector<MeshVertex>& vertices, const std::vector<unsigned int>& indices, const std::string& texture);

    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;

    std::string texture;

    ShaderMod meshMod = ShaderMod::NONE;
    float modValue = 0;
};



#endif //ZEUS_SERVERMESHPART_H

#pragma clang diagnostic pop