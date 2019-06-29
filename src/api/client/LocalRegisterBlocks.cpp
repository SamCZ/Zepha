//
// Created by aurailus on 28/06/19.
//

#include "LocalRegisterBlocks.h"

LocalRegisterBlocks::LocalRegisterBlocks(sol::table& zeus, LocalDefs &defs) {
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

        //TODO: Try to make a constructor or *something*
        LocalBlockModel model = LocalBlockModel::create(*modelOpt, *texturesOpt, defs.textures(), visible, culls);
        LocalBlockDef blockDef(identifier, defs.blocks().definitionsSize(), std::move(model), solid, sBoxes[0]);

        //Bind Callbacks
        auto on_place_client = blockTable.get<sol::optional<sol::function>>("on_place_client");
        if (on_place_client) blockDef.callbacks.insert({Callback::PLACE_CLIENT, *on_place_client});

        auto on_break_client = blockTable.get<sol::optional<sol::function>>("on_break_client");
        if (on_break_client) blockDef.callbacks.insert({Callback::BREAK_CLIENT, *on_break_client});

        //Add Block Definition to the Atlas
        defs.blocks().registerBlock(std::move(blockDef));
    }
}
