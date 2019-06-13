//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_MODULESERVERREGISTERBLOCK_H
#define ZEUS_MODULESERVERREGISTERBLOCK_H

#include "../../def/ServerDefs.h"
#include "../../def/block/SelectionBox.h"
#include "../../def/block/client/LocalBlockDef.h"
#include "../../def/block/client/LocalBlockAtlas.h"
#include <sol.hpp>

class ModuleServerRegisterBlock {
public:
    ModuleServerRegisterBlock(sol::state& lua, sol::table& zeus, ServerDefs& defs) {
        zeus.set_function("register_block", [&](std::string identifier, sol::table data) {
            try {
                if (identifier.length() == 0) throw "No Identifier";

                auto name       = data.get<sol::optional<std::string>>("name");
                auto textures   = data.get<sol::optional<sol::table>>("textures");
                auto modelStr   = data.get_or<std::string>("model", "default:cube");
                auto selection  = data.get<sol::optional<sol::table>>("selection_box");

                bool visible    = data.get_or("visible", true);
                bool culls      = data.get_or("culls", true);
                bool solid      = data.get_or("solid", true);

                if (!name) throw "No name property";
                if (!textures) throw "No textures table";

                //Convert textures to a std::vector
                std::vector<std::string> texturesVector;
                textures->for_each([&](sol::object key, sol::object value) {
                    if (!value.is<std::string>()) throw "Non string texture value.";
                    std::string str = value.as<std::string>();
                    texturesVector.push_back(str);
                });
                if (texturesVector.size() == 0) texturesVector.push_back("_missing");

                sol::table models = zeus["registered_blockmodels"];
                auto model = models.get<sol::optional<sol::table>>(modelStr);

                if (!model) throw "Undefined model";

                //TODO: Validate and allow multiple selection boxes
                SelectionBox sbox = {{0, 0, 0}, {1, 1, 1}};
                if (selection) {
                    sol::table def = (*selection)[1];
                    sbox = {{def[1], def[2], def[3]}, {def[4], def[5], def[6]}};
                }

                ServerBlockModel blockModel = ServerBlockModel::create(*model, texturesVector, visible, culls);
                ServerBlockDef def(identifier, defs.blocks().definitionsSize(), std::move(blockModel), solid, sbox);

                //Add Callbacks to the block definition
                auto on_place = data.get<sol::optional<sol::function>>("on_place");
                if (on_place) def.callbacks.insert({Callback::PLACE, *on_place});

                auto on_break = data.get<sol::optional<sol::function>>("on_break");
                if (on_break) def.callbacks.insert({Callback::BREAK, *on_break});

                auto on_construct = data.get<sol::optional<sol::function>>("on_construct");
                if (on_construct) def.callbacks.insert({Callback::CONSTRUCT, *on_construct});

                auto after_construct = data.get<sol::optional<sol::function>>("after_construct");
                if (after_construct) def.callbacks.insert({Callback::AFTER_CONSTRUCT, *after_construct});

                auto on_destruct = data.get<sol::optional<sol::function>>("on_destruct");
                if (on_destruct) def.callbacks.insert({Callback::DESTRUCT, *on_destruct});

                auto after_destruct = data.get<sol::optional<sol::function>>("after_destruct");
                if (after_destruct) def.callbacks.insert({Callback::AFTER_DESTRUCT, *after_destruct});

                //Add definition to the list
                defs.blocks().registerBlock(std::move(def));
            }
            catch (const std::string& e) {
                std::cout << Log::err << "Exception on register_block: " << e << Log::endl;
            }
        });
    }
};


#endif //ZEUS_MODULESERVERREGISTERBLOCK_H
