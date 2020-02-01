//
// Created by aurailus on 10/06/19.
//

#pragma once

#include <string>
#include <sol2/sol.hpp>
#include "SelectionBox.h"
#include "BlockModel.h"
#include "../ItemDef.h"
#include "../../lua/Callback.h"
#include "../../game/entity/Model.h"
#include "../../game/graph/meshtypes/GuiMesh.h"

class BlockDef : public ItemDef {
public:
    BlockDef() = default;
    BlockDef(const std::string& identifier, const std::string& name, unsigned short maxStackSize,
            const BlockModel& model, bool solid, const std::vector<SelectionBox>& sBoxes, const std::vector<SelectionBox>& cBoxes);

    BlockDef(const std::string& identifier, unsigned int index, const std::string& name,
            unsigned short maxStackSize, const BlockModel& model, bool solid,
            const std::vector<SelectionBox>& sBoxes, const std::vector<SelectionBox>& cBoxes);

    void createModel();

    BlockModel model;
    bool culls = false;
    bool solid = false;

    std::vector<SelectionBox> sBoxes;
    std::vector<SelectionBox> cBoxes;
    std::shared_ptr<Model> entityModel = std::make_shared<Model>();
    std::shared_ptr<GuiMesh> guiModel = std::make_shared<GuiMesh>();

    std::unordered_map<Callback, sol::function, Util::EnumClassHash> callbacks {};
};
