//
// Created by aurailus on 13/08/19.
//

#include "DefinitionAtlas.h"

#include "game/def/ItemDef.h"
#include "game/def/BlockDef.h"
#include "game/def/CraftItemDef.h"

ItemDef& DefinitionAtlas::fromId(unsigned int id) const {
    if (id > defs.size()) throw std::runtime_error("Undefined definition id " + std::to_string(id) + " requested.");
    return *defs.at(static_cast<unsigned long>(id));
}

ItemDef &DefinitionAtlas::fromStr(const std::string& identifier) const {
    if (identifier == "") return *defs.at(AIR);
    if (defTable.count(identifier) <= 0) throw std::runtime_error("Undefined definition identifier " + identifier + " requested.");
    return *defs.at(static_cast<unsigned long>(defTable.at(identifier)));
}

BlockDef& DefinitionAtlas::blockFromId(unsigned int index) const {
    ItemDef& def = fromId(index);
    if (def.type != ItemDef::Type::BLOCK) throw std::runtime_error(std::to_string(index) + " is not a block.");
    return static_cast<BlockDef&>(def);
}

BlockDef &DefinitionAtlas::blockFromStr(const std::string& identifier) const {
    ItemDef& def = fromStr(identifier);
    if (def.type != ItemDef::Type::BLOCK) throw std::runtime_error(identifier + " is not a block.");
    return static_cast<BlockDef&>(def);
}

CraftItemDef& DefinitionAtlas::craftItemFromId(unsigned int index) const {
    ItemDef& def = fromId(index);
    if (def.type != ItemDef::Type::CRAFTITEM) throw std::runtime_error(std::to_string(index) + " is not a craftitem.");
    return static_cast<CraftItemDef&>(def);
}

CraftItemDef &DefinitionAtlas::craftItemFromStr(const std::string& identifier) const {
    ItemDef& def = fromStr(identifier);
    if (def.type != ItemDef::Type::CRAFTITEM) throw std::runtime_error(identifier + " is not a craftitem.");
    return static_cast<CraftItemDef&>(def);
}

unsigned int DefinitionAtlas::size() {
    return static_cast<unsigned int>(defs.size());
}

DefinitionAtlas::~DefinitionAtlas() {
    for (auto def : defs) delete def;
}