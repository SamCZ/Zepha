//
// Created by aurailus on 2019-12-11.
//

#pragma once

#include <memory>

#include "SubgameConfig.h"

class AtlasRef;

class Subgame {
public:
    std::shared_ptr<AtlasRef> iconRef = nullptr;
    SubgameConfig config {};
    std::string subgamePath = "";
};
