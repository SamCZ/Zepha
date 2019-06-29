//
// Created by aurailus on 04/12/18.
//

#ifndef GLPROJECT_BLOCKMODEL_H
#define GLPROJECT_BLOCKMODEL_H

#include <vector>
#include <sol.hpp>
#include <set>
#include "LocalMeshPart.h"
#include "../../../game/scene/world/graph/MeshVertex.h"
#include "../../../util/Dir.h"

struct LocalBlockModel {
    std::array<std::vector<LocalMeshPart>, 7> parts;
    std::vector<std::pair<MeshMod, float>> meshMods;
    std::set<std::shared_ptr<AtlasRef>> textureRefs;

    bool culls;
    bool visible;

    static LocalBlockModel create(sol::table model, sol::table texturesTbl, TextureAtlas& atlas, bool visible, bool culls);
};


#endif //GLPROJECT_BLOCKMODEL_H
