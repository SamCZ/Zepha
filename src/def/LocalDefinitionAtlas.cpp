//
// Created by aurailus on 02/12/18.
//

#include "LocalDefinitionAtlas.h"

LocalDefinitionAtlas::LocalDefinitionAtlas() {
    BlockModel nullModel;
    nullModel.visible = false,
    nullModel.culls = false;

    //Invalid Node
    BlockDef* invalid = new BlockDef("invalid", 0, nullModel, true, {{0, 0, 0}, {1, 1, 1}});
    defs.push_back(invalid);
    defTable.insert({"invalid", 0});

    //Air Node
    BlockDef* air = new BlockDef("air", 0, nullModel, false, {{0, 0, 0}, {1, 1, 1}});
    defs.push_back(air);
    defTable.insert({"air", 1});
}

void LocalDefinitionAtlas::setIdentifiers(std::vector<std::string> &identifiers) {
    for (unsigned int i = 0; i < identifiers.size(); i++) {
        defs.emplace_back();
        defTable.insert({identifiers[i], i});
    }
}

void LocalDefinitionAtlas::registerDef(ItemDef* def) {
    if (!defTable.count(def->identifier)) {
        std::cout << Log::err << "Client/Server block identifier desync: " + def->identifier + ". Exiting." << Log::endl;
        exit(1);
    }
    def->index = defTable[def->identifier];
    defs[def->index] = def;
}
