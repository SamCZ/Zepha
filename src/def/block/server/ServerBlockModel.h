//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_SERVERBLOCKMODEL_H
#define ZEUS_SERVERBLOCKMODEL_H

#include <vector>
#include <sol2/sol.hpp>
#include <set>
#include "ServerMeshPart.h"
#include "../../../game/scene/world/graph/MeshVertex.h"
#include "../../../util/Dir.h"

struct ServerBlockModel {
    std::array<std::vector<ServerMeshPart>, 7> parts;
    std::set<std::shared_ptr<AtlasRef>> textureRefs;

    bool culls;
    bool visible;

    static ServerBlockModel create(sol::table model, sol::table texturesTbl, bool visible, bool culls);
};

#endif //ZEUS_SERVERBLOCKMODEL_H
