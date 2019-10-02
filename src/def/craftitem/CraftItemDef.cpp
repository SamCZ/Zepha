//
// Created by aurailus on 01/10/19.
//

#include "CraftItemDef.h"

CraftItemDef::CraftItemDef(const std::string &identifier, const std::string &name,
        const std::vector<std::string>& textures, const std::vector<sptr<AtlasRef>>& textureRefs) :

    CraftItemDef(identifier, 0, name, textures, textureRefs) {}

CraftItemDef::CraftItemDef(const std::string& identifier, unsigned int index, const std::string& name,
        const std::vector<std::string>& textures, const std::vector<sptr<AtlasRef>>& textureRefs) :

    ItemDef {identifier, name, index, ItemDef::Type::CRAFTITEM},
    textures(textures),
    textureRefs(textureRefs) {}
