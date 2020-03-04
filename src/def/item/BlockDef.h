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
    BlockDef() = default;
    BlockDef(const std::string& identifier, const std::string& name, unsigned short maxStackSize,
            const BlockModel& model, const BlockModel& farModel, bool solid, const std::vector<SelectionBox>& sBoxes,
            const std::vector<SelectionBox>& cBoxes);

    BlockDef(const std::string& identifier, unsigned int index, const std::string& name,
            unsigned short maxStackSize, const BlockModel& model, const BlockModel& farModel, bool solid,
            const std::vector<SelectionBox>& sBoxes, const std::vector<SelectionBox>& cBoxes);

    void createModel();

    BlockModel model, farModel;
    bool culls = false;
    bool solid = false;

    std::vector<SelectionBox> sBoxes;
    std::vector<SelectionBox> cBoxes;

    std::unordered_map<Callback, sol::function, Util::EnumClassHash> callbacks {};
};
