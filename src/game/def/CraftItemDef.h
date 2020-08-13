//
// Created by aurailus on 01/10/19.
//

#pragma once

#include <vector>

#include "ItemDef.h"

class AtlasRef;
class TextureAtlas;

class CraftItemDef : public ItemDef {
public:
    CraftItemDef() = default;
    CraftItemDef(const std::string& identifier, const std::string& name, unsigned short maxStackSize,
            const std::vector<std::string>& textures, const std::vector<std::shared_ptr<AtlasRef>>& textureRefs);

    CraftItemDef(const std::string& identifier, unsigned int index, const std::string& name,
            unsigned short maxStackSize, const std::vector<std::string>& textures, const std::vector<std::shared_ptr<AtlasRef>>& textureRefs);

    void createModel(TextureAtlas& atlas);

    std::vector<std::string> textures {};
    std::vector<std::shared_ptr<AtlasRef>> textureRefs {};
};
