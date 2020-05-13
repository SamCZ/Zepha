//
// Created by aurailus on 10/06/19.
//

#pragma once

#include <string>
#include <sol2/sol.hpp>

#include "../ItemDef.h"

#include "BlockModel.h"
#include "SelectionBox.h"
#include "../../lua/Callback.h"

class BlockDef : public ItemDef {
public:
    BlockDef(
        const std::string& identifier,
        const std::string& name,
        unsigned short maxStackSize,
        const BlockModel& model,
        const BlockModel& farModel,
        bool solid,
        glm::ivec3 lightSource,
        const std::vector<SelectionBox>& sBoxes,
        const std::vector<SelectionBox>& cBoxes,
        unsigned int index = 0
    );

    void createModel();

    BlockModel model;
    BlockModel farModel;

    bool culls = false;
    bool solid = false;

    glm::ivec3 lightSource;

    std::vector<SelectionBox> sBoxes;
    std::vector<SelectionBox> cBoxes;

    std::unordered_map<Callback, sol::function, Util::EnumClassHash> callbacks {};
};
