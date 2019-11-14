//
// Created by aurailus on 2019-11-12.
//

#include "ServerRegisterBiomes.h"
#include "../../def/gen/BiomeDef.h"

ServerRegisterBiomes::ServerRegisterBiomes(sol::table& core, ServerDefs &defs) {
    //Register all of the biomes in the zepha.registered_biomes table.
    for (auto biomeRef : core.get<sol::table>("registered_biomes")) {
        //Get the unique identifier of the biome
        std::string identifier = biomeRef.first.as<std::string>();

        //Make sure the biome table is actually a table, and get it.
        if (!biomeRef.second || !biomeRef.second.is<sol::table>()) throw identifier + "'s definition table is not a table!";
        sol::table biomeTable = biomeRef.second.as<sol::table>();

        //Get biome properties and throw errors if required ones are missing
        auto temperature  = biomeTable.get_or<float>("temperature", -10000);
        auto humidity     = biomeTable.get_or<float>("humidity",    -10000);
        auto roughness    = biomeTable.get_or<float>("roughness",   -10000);

        if (temperature == -10000) throw identifier + " is missing temperature property!";
        if (temperature < -1 || temperature > 1) throw identifier + "'s temperature property is out of range!";
        if (humidity == -10000) throw identifier + " is missing humidity property!";
        if (humidity < 0 || humidity > 1) throw identifier + "'s humidity property is out of range!";
        if (roughness == -10000) throw identifier + " is missing roughness property!";
        if (roughness < 0 || roughness > 1) throw identifier + "'s roughness property is out of range!";

        //Get the blocks for the terrain to be made out of.
        auto blocksList   = biomeTable.get<sol::optional<sol::table>>("blocks");
        if (!blocksList) throw identifier + "'s definition is missing blocks list.";

        auto topBlock  = (*blocksList).get<sol::optional<std::string>>("top");
        auto soilBlock = (*blocksList).get<sol::optional<std::string>>("soil");
        auto rockBlock = (*blocksList).get<sol::optional<std::string>>("rock");

        if (!topBlock)  throw identifier + "'s blocks property is missing a `top` block.";
        if (!soilBlock) throw identifier + "'s blocks property is missing a `soil` block.";
        if (!rockBlock) throw identifier + "'s blocks property is missing a `rock` block.";

        BiomeDef* biomeDef = new BiomeDef(
            identifier, temperature, humidity, roughness,
            defs.defs().blockFromStr(*topBlock).index,
            defs.defs().blockFromStr(*soilBlock).index,
            defs.defs().blockFromStr(*rockBlock).index
        );

        Add Biome Definition to the Gen
        defs.gen().registerBiome(biomeDef);
    }
}
