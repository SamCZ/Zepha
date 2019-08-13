//
// Created by aurailus on 13/08/19.
//

#ifndef ZEUS_BLOCKMODEL_H
#define ZEUS_BLOCKMODEL_H

#include <set>
#include <vector>
#include "LocalMeshPart.h"

struct BlockModel {
    std::array<std::vector<LocalMeshPart>, 7> parts;
    std::vector<std::pair<MeshMod, float>> meshMods;
    std::set<std::shared_ptr<AtlasRef>> textureRefs;

    bool culls;
    bool visible;
};

#endif //ZEUS_BLOCKMODEL_H
