//
// Created by aurailus on 01/10/19.
//

#include "ServerRegisterItems.h"

ServerRegisterItems::ServerRegisterItems(sol::table& core, ServerDefs &defs) {
    //Register all of the items in the zepha.registered_items table.
    for (auto itemRef : core.get<sol::table>("registered_items")) {
        //Get the unique identifier of the item
        std::string identifier = itemRef.first.as<std::string>();

        //Make sure the block table is actually a table, and get it.
        if (!itemRef.second || !itemRef.second.is<sol::table>()) throw identifier + "'s definition table is not a table!";
        sol::table itemTable = itemRef.second.as<sol::table>();

        //Get item properties and throw errors if required ones are missing
        auto nameOpt      = itemTable.get<sol::optional<std::string>>("name");
        auto texturesOpt  = itemTable.get<sol::optional<sol::table>> ("textures");

        if (!nameOpt) throw identifier + " is missing name property!";
        if (!texturesOpt) throw identifier + " is missing textures property!";

        auto maxStack = itemTable.get_or("stack", 64);

        //Convert Textures Table to Vector
        std::vector<std::string> textures;
        for (auto pair : *texturesOpt) {
            if (!pair.second.is<std::string>()) throw "Textures table has non-string value!";
            textures.push_back(pair.second.as<std::string>());
        }
        if (textures.size() == 0) textures.push_back("_missing");

        CraftItemDef* itemDef = new CraftItemDef(identifier, defs.defs.size(), *nameOpt, maxStack, textures, {});

        //Add Block Definition to the Atlas
        defs.defs.registerDef(itemDef);
    }
}
