//
// Created by aurailus on 2020-01-10.
//

#pragma once

#include "../Lua.h"
#include "../../def/LocalSubgame.h"
#include "../../def/gen/BiomeDef.h"
#include "../../def/ServerSubgame.h"
#include "../../def/item/CraftItemDef.h"
#include "../../def/LocalDefinitionAtlas.h"
#include "../../def/ServerDefinitionAtlas.h"

namespace RegisterItems {
    static void registerItems(sol::table source, DefinitionAtlas& defs, TextureAtlas* atlas) {
        // Parses through all of the zepha.registered_items and makes ItemDefs.
        for (auto itemRef : source) {

            // Validate that the identifier and definition table exist
            std::string identifier = itemRef.first.as<std::string>();

            if (!itemRef.second || !itemRef.second.is<sol::table>())
                throw identifier + "register_item expects a table as the second paremeter";
            sol::table itemTable = itemRef.second.as<sol::table>();

            // Basic item properties
            auto nameOpt      = itemTable.get<sol::optional<std::string>>("name");
            auto texturesOpt  = itemTable.get<sol::optional<sol::table>> ("textures");
            auto maxStack     = itemTable.get_or("stack", 64);

            if (!nameOpt) throw std::runtime_error(identifier + " is missing name property!");
            if (!texturesOpt) throw std::runtime_error(identifier + " is missing textures property!");

            //Convert Textures Table to Vector
            std::vector<std::string> textures;
            std::vector<std::shared_ptr<AtlasRef>> textureRefs;
            for (auto pair : *texturesOpt) {
                if (!pair.second.is<std::string>()) throw std::runtime_error("textures table has non-string value");
                textures.push_back(pair.second.as<std::string>());
                if (atlas) textureRefs.push_back((*atlas)[pair.second.as<std::string>()]);
            }
            if (textures.size() == 0) {
                textures.push_back("_missing");
                if (atlas) textureRefs.push_back((*atlas)["_missing"]);
            }

            // Creat the definition
            CraftItemDef* itemDef = new CraftItemDef(
                identifier,
                defs.size(),
                *nameOpt,
                maxStack,
                textures,
                textureRefs
            );

            if (atlas) itemDef->createModel(*atlas);
            defs.registerDef(itemDef);
        }
    }

    static void server(sol::table& core, ServerSubgame& game) {
        registerItems(core.get<sol::table>("registered_items"), game.getDefs(), nullptr);
    }

    static void client(sol::table& core, LocalSubgame& game) {
        registerItems(core.get<sol::table>("registered_items"), game.getDefs(), &game.textures);
    }
};