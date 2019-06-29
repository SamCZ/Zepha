//
// Created by aurailus on 29/06/19.
//

#include "ServerRegisterBlocks.h"

ServerRegisterBlocks::ServerRegisterBlocks(sol::table& zeus, ServerDefs &defs) {
    for (auto blockRef : zeus.get<sol::table>("registered_blocks")) {
        std::string identifier = blockRef.first.as<std::string>();

        if (!blockRef.second || !blockRef.second.is<sol::table>())
            throw identifier + "'s definition table is not a table!";

        //Get Properties
        sol::table blockTable = blockRef.second.as<sol::table>();

        auto nameOpt      = blockTable.get<sol::optional<std::string>>("name");
        auto modelStrOpt  = blockTable.get<sol::optional<std::string>>("model");
        auto texturesOpt  = blockTable.get<sol::optional<sol::table>> ("textures");
        auto selectionOpt = blockTable.get<sol::optional<sol::table>> ("selection_box");

        if (!nameOpt) throw identifier + " is missing name property!";
        if (!texturesOpt) throw identifier + " is missing textures property!";

        bool visible = blockTable.get_or("visible", true);
        bool culls   = blockTable.get_or("culls", true);
        bool solid   = blockTable.get_or("solid", true);

        std::string modelStr = (modelStrOpt ? *modelStrOpt : "default:cube");
        sol::optional<sol::table> modelOpt = zeus.get<sol::table>("registered_blockmodels").get<sol::table>(modelStr);

        if (!modelOpt) throw identifier + " specifies invalid model " + modelStr + "!";

        std::vector<SelectionBox> sBoxes;
        if (selectionOpt) {
            for (auto pair : *selectionOpt) {
                sol::table s = pair.second;
                sBoxes.push_back({{s[1], s[2], s[3]}, {s[4], s[5], s[6]}});
            }
        } else sBoxes.push_back({{0, 0, 0}, {1, 1, 1}});

        //TODO: Make ServerBlockDef take multiple selectionboxes
        ServerBlockModel model = ServerBlockModel::create(*modelOpt, *texturesOpt, visible, culls);
        ServerBlockDef blockDef(identifier, defs.blocks().definitionsSize(), std::move(model), solid, sBoxes[0]);

        //Bind Callbacks
        auto on_place = blockTable.get<sol::optional<sol::function>>("on_place");
        if (on_place) blockDef.callbacks.insert({Callback::PLACE, *on_place});

        auto on_break = blockTable.get<sol::optional<sol::function>>("on_break");
        if (on_break) blockDef.callbacks.insert({Callback::BREAK, *on_break});

        auto on_construct = blockTable.get<sol::optional<sol::function>>("on_construct");
        if (on_construct) blockDef.callbacks.insert({Callback::CONSTRUCT, *on_construct});

        auto after_construct = blockTable.get<sol::optional<sol::function>>("after_construct");
        if (after_construct) blockDef.callbacks.insert({Callback::AFTER_CONSTRUCT, *after_construct});

        auto on_destruct = blockTable.get<sol::optional<sol::function>>("on_destruct");
        if (on_destruct) blockDef.callbacks.insert({Callback::DESTRUCT, *on_destruct});

        auto after_destruct = blockTable.get<sol::optional<sol::function>>("after_destruct");
        if (after_destruct) blockDef.callbacks.insert({Callback::AFTER_DESTRUCT, *after_destruct});

        //Add Block Definition to the Atlas
        defs.blocks().registerBlock(std::move(blockDef));
    }
}
