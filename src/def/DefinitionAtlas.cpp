//
// Created by aurailus on 13/08/19.
//

#include "DefinitionAtlas.h"

ItemDef& DefinitionAtlas::fromId(unsigned int id) const {
    if (id > defs.size()) {
        std::cout << Log::err << "Undefined def #" << id << " requested, returning invalid." << Log::endl;
        return *defs.at(INVALID);
    }

    return *defs.at(static_cast<unsigned long>(id));
}

ItemDef &DefinitionAtlas::fromStr(const std::string& identifier) const {
    if (identifier == "") return *defs.at(AIR);
    if (defTable.count(identifier) <= 0) {
        std::cout << Log::err << "Undefined identifier \"" << identifier << "\" requested, returning invalid." << Log::endl;
        return *defs.at(INVALID);
    }

    return *defs.at(static_cast<unsigned long>(defTable.at(identifier)));
}

BlockDef& DefinitionAtlas::blockFromId(unsigned int index) const {
    ItemDef& def = fromId(index);
    if (def.type != ItemDef::Type::BLOCK) {
        std::cout << Log::err << "Invalid block id." << Log::endl;
        exit(0);
    }
    return static_cast<BlockDef&>(def);
}

BlockDef &DefinitionAtlas::blockFromStr(const std::string& identifier) const {
    ItemDef& def = fromStr(identifier);
    if (def.type != ItemDef::Type::BLOCK) {
        std::cout << Log::err << "Invalid block id." << Log::endl;
        exit(0);
    }
    return static_cast<BlockDef&>(def);
}

CraftItemDef& DefinitionAtlas::craftItemFromId(unsigned int index) const {
    ItemDef& def = fromId(index);
    if (def.type != ItemDef::Type::CRAFTITEM) {
        std::cout << Log::err << "Invalid item id." << Log::endl;
        exit(0);
    }
    return static_cast<CraftItemDef&>(def);
}

CraftItemDef &DefinitionAtlas::craftItemFromStr(const std::string& identifier) const {
    ItemDef& def = fromStr(identifier);
    if (def.type != ItemDef::Type::CRAFTITEM) {
        std::cout << Log::err << "Invalid item id." << Log::endl;
        exit(0);
    }
    return static_cast<CraftItemDef&>(def);
}

unsigned int DefinitionAtlas::size() {
    return static_cast<unsigned int>(defs.size());
}

DefinitionAtlas::~DefinitionAtlas() {
    for (auto def : defs) delete def;
}