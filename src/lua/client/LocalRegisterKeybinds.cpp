//
// Created by aurailus on 12/10/19.
//

#include "LocalRegisterKeybinds.h"

LocalRegisterKeybinds::LocalRegisterKeybinds(sol::table& core, LocalDefs &defs) {
    //Register all of the items in the zepha.registered_keybinds table.
    for (auto keybindRef : core.get<sol::table>("registered_keybinds")) {
        //Get the unique identifier of the item
        std::string identifier = keybindRef.first.as<std::string>();
        //Make sure the keybind table is actually a table, and get it.
        if (!keybindRef.second || !keybindRef.second.is<sol::table>()) throw identifier + "'s definition table is not a table!";
        sol::table keybindTbl = keybindRef.second.as<sol::table>();

        std::cout << identifier << ": " << keybindTbl.get<std::string>("default") << std::endl;

//        for ()
//
//        //Get item properties and throw errors if required ones are missing
//        auto nameOpt      = itemTable.get<sol::optional<std::string>>("name");
//        auto texturesOpt  = itemTable.get<sol::optional<sol::table>> ("textures");
//
//        if (!nameOpt) throw identifier + " is missing name property!";
//        if (!texturesOpt) throw identifier + " is missing textures property!";
//
//        //Convert Textures Table to Vector
//        std::vector<std::string> textures;
//        std::vector<sptr<AtlasRef>> textureRefs;
//        for (auto pair : *texturesOpt) {
//            if (!pair.second.is<std::string>()) throw "Textures table has non-string value!";
//            textures.push_back(pair.second.as<std::string>());
//            textureRefs.push_back(defs.textures().getTextureRef(pair.second.as<std::string>()));
//        }
//        if (textures.size() == 0) {
//            textures.push_back("_missing");
//            textureRefs.push_back(defs.textures().getTextureRef("_missing"));
//        }
//
//        CraftItemDef* itemDef = new CraftItemDef(identifier, defs.defs().size(), *nameOpt, textures, textureRefs);
//        itemDef->createModel(defs.textures());
//
//        //Add Block Definition to the Atlas
//        defs.defs().registerDef(itemDef);
    }
}
