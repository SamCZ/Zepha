//
// Created by aurailus on 13/08/19.
//

#pragma once

#include <set>
#include <vector>

#include "MeshPart.h"

struct BlockModel {
    std::array<std::vector<MeshPart>, 7> parts {};
    std::vector<std::pair<MeshMod, float>> meshMods {};
    std::set<std::shared_ptr<AtlasRef>> textureRefs {};

    bool culls = false;
    bool visible = false;

    static BlockModel createCube(std::vector<std::shared_ptr<AtlasRef>> textureRefs,
            std::vector<unsigned int> blendInds, std::vector<std::shared_ptr<AtlasRef>> blendMaskRefs);
};