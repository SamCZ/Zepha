//
// Created by aurailus on 26/08/19.
//

#pragma once

#include <string>
#include <vector>
#include "AnimChannel.h"

class ModelAnimation {
public:
    ModelAnimation() = default;
    explicit ModelAnimation(const std::string& name);

    std::string name = "";
    std::vector<AnimChannel> channels {};
    uint duration = 0;
    double ticksPerSecond = 0;
};
