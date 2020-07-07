//
// Created by aurailus on 02/12/18.
//

#include "LocalDefinitionAtlas.h"

#include "item/BlockDef.h"
#include "texture/TextureAtlas.h"

LocalDefinitionAtlas::LocalDefinitionAtlas(TextureAtlas& atlas) {
    //Invalid Node
    BlockModel invalidModel = BlockModel::createCube({atlas["_missing"]}, {}, {});
    BlockDef* invalid = new BlockDef("invalid", "Invalid (you broke the game!)", 64, invalidModel, invalidModel, true, {}, false, {{}}, {{}});
    defs.push_back(invalid);
    defTable.insert({"invalid", 0});

    //Air Node
    BlockModel nullModel {};
    BlockDef* air = new BlockDef("air", "Air", 64, nullModel, nullModel, false, {}, true, {}, {}, 1);
    defs.push_back(air);
    defTable.insert({"air", 1});
}

void LocalDefinitionAtlas::setIdentifiers(const std::vector<std::string>& identifiers) {
    for (unsigned int i = 0; i < identifiers.size(); i++) {
        if (i >= 2) defs.emplace_back(); //Don't emplace air or ignore
        defTable.insert({identifiers[i], i});
    }
}

void LocalDefinitionAtlas::registerDef(ItemDef* def) {
    if (!defTable.count(def->identifier)) throw std::runtime_error("Client/Server definition desync: " + def->identifier + ".");
    def->index = defTable[def->identifier];
    defs[def->index] = def;
}
