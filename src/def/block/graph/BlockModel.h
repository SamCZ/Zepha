//
// Created by aurailus on 04/12/18.
//

#ifndef GLPROJECT_BLOCKMODEL_H
#define GLPROJECT_BLOCKMODEL_H

#include <vector>
#include <sol.hpp>
#include "MeshPart.h"
#include "../../../game/scene/world/graph/MeshVertex.h"

struct BlockModel {
    std::vector<MeshPart> leftFaces;
    std::vector<MeshPart> rightFaces;
    std::vector<MeshPart> topFaces;
    std::vector<MeshPart> bottomFaces;
    std::vector<MeshPart> frontFaces;
    std::vector<MeshPart> backFaces;
    std::vector<MeshPart> noCulledFaces;

    bool culls;
    bool visible;

    static BlockModel from_lua_def(sol::table model, sol::table textures, TextureAtlas& atlas, bool visible, bool culls);
};


#endif //GLPROJECT_BLOCKMODEL_H
