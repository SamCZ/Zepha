//
// Created by aurailus on 12/10/19.
//

#include "LocalRegisterKeybinds.h"

LocalRegisterKeybinds::LocalRegisterKeybinds(sol::table& core, LocalDefs &defs, LuaInputManager& manager) {
    //Register all of the items in the zepha.registered_keybinds table.
    for (auto keybindRef : core.get<sol::table>("registered_keybinds")) {
        //Get the unique identifier of the item
        std::string identifier = keybindRef.first.as<std::string>();
        //Make sure the keybind table is actually a table, and get it.
        if (!keybindRef.second || !keybindRef.second.is<sol::table>()) throw identifier + "'s definition table is not a table!";
        sol::table keybindTbl = keybindRef.second.as<sol::table>();
        ushort def = keybindTbl.get<ushort>("default");

        auto onPress = keybindTbl.get<sol::optional<sol::function>>("on_press");
        auto onRelease = keybindTbl.get<sol::optional<sol::function>>("on_release");

        if (onPress) manager.bindOnDown(def, *onPress);
        if (onRelease) manager.bindOnUp(def, *onRelease);
    }
}
