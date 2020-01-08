//
// Created by aurailus on 2020-01-07.
//

#include "ServerLocalLuaEntity.h"

ServerLocalLuaEntity::ServerLocalLuaEntity(unsigned int id, LocalDefs &defs, const std::string &appearance,
    const std::string &arg1, const std::string &arg2) :

    id(id),
    defs(defs) {

    setDisplayType(appearance, arg1, arg2);
}

void ServerLocalLuaEntity::setDisplayType(const std::string &type, const std::string &arg, const std::string &arg2) {
    if (strncmp(type.data(), "gameobject", 10) == 0 &&
       (strncmp(displayType.data(), "gameobject", 10) || arg2 != displayArg2)) {

        ItemDef& def = defs.defs.fromStr(arg2);

        if (def.type == ItemDef::Type::BLOCK)
            entity->setModel(static_cast<BlockDef&>(def).entityModel);
        else if (def.type == ItemDef::Type::CRAFTITEM)
            entity->setModel(static_cast<CraftItemDef&>(def).entityModel);
    }
    else if (strncmp(type.data(), "model", 5) == 0 && !arg2.empty() &&
            (strncmp(displayType.data(), "model", 5) || arg != displayArg1 || arg != displayArg2)) {
        auto model = std::make_shared<Model>();
        model->fromSerialized(defs.models.models[arg], {defs.textures[arg2]});
        entity->setModel(model);
    }
    else return;

    displayType = type;
    displayArg1 = arg;
    displayArg2 = arg2;
}
