//
// Created by aurailus on 01/10/19.
//

#pragma once

#include <vector>
#include "../ItemDef.h"
#include "../../util/Pointer.h"
#include "../texture/AtlasRef.h"
#include "../../game/entity/Model.h"

class CraftItemDef : public ItemDef {
public:
    CraftItemDef() = default;
    CraftItemDef(const std::string& identifier, const std::string& name, unsigned short maxStackSize,
            const std::vector<std::string>& textures, const std::vector<sptr<AtlasRef>>& textureRefs);

    CraftItemDef(const std::string& identifier, unsigned int index, const std::string& name,
            unsigned short maxStackSize, const std::vector<std::string>& textures, const std::vector<sptr<AtlasRef>>& textureRefs);

    void createModel(TextureAtlas& atlas);

    std::vector<std::string> textures {};
    std::vector<sptr<AtlasRef>> textureRefs {};

    sptr<Model> entityModel = std::make_shared<Model>();
};
