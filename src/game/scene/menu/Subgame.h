//
// Created by aurailus on 2019-12-11.
//

#pragma once

#include <string>
#include <memory>
#include "SubgameConfig.h"
#include "../../../def/texture/AtlasRef.h"

class Subgame {
public:
    std::shared_ptr<AtlasRef> iconRef = nullptr;
    SubgameConfig config {};
    std::string subgamePath = "";
};
