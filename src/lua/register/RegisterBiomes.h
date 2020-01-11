//
// Created by aurailus on 2020-01-10.
//

#pragma once

#include <sol2/sol.hpp>
#include <noise/module/modulebase.h>
#include <noise/module/add.h>
#include <noise/module/module.h>
#include "../../def/LocalDefs.h"
#include "../../def/ServerDefs.h"
#include "../../def/gen/BiomeDef.h"

namespace RegisterBiomes {
    static noise::module::Module* parseNoise(std::vector<noise::module::Module*>& modules, sol::table noise) {
        std::string type = noise["module"];

        if (type == "add") {
            auto module = new noise::module::Add();
            sol::table sources = noise["sources"];

            auto mod0 = parseNoise(modules, sources[1]);
            auto mod1 = parseNoise(modules, sources[2]);
            module->SetSourceModule(0, *mod0);
            module->SetSourceModule(1, *mod1);

            modules.push_back(module);
            return module;
        }
        else if (type == "scale_bias") {
            auto module = new noise::module::ScaleBias();
            sol::table source = noise["source"];

            auto mod = parseNoise(modules, source);
            module->SetSourceModule(0, *mod);

            module->SetScale(noise.get_or<float>("scale", noise::module::DEFAULT_SCALE));
            module->SetBias(noise.get_or<float>("bias", noise::module::DEFAULT_BIAS));

            modules.push_back(module);
            return module;
        }
        else if (type == "perlin") {
            auto module = new noise::module::Perlin();

            module->SetSeed(noise.get_or<float>("seed", 0));
            module->SetOctaveCount(noise.get_or<float>("octaves", 0));
            module->SetFrequency(noise.get_or<float>("frequency", 0));
            module->SetLacunarity(noise.get_or<float>("lacunarity", 0));
            module->SetPersistence(noise.get_or<float>("persistence", 0));

            modules.push_back(module);
            return module;
        }
        else if (type == "voronoi") {
            auto module = new noise::module::Voronoi();

            module->SetSeed(noise.get_or<float>("seed", 0));
            module->SetDisplacement(noise.get_or<float>("displacement", 0));
            module->SetFrequency(noise.get_or<float>("frequency", 0));

            modules.push_back(module);
            return module;
        }
    }

    static void registerBiomes(sol::table source, DefinitionAtlas& defs, BiomeAtlas& biomes) {
        // Parses through all of the zepha.registered_biomes and makes BiomeDefs.
        for (auto biomeRef : source) {

            // Validate that the identifier and definition table exist
            std::string identifier = biomeRef.first.as<std::string>();

            if (!biomeRef.second || !biomeRef.second.is<sol::table>())
                throw "register_biome expects a table as the second parameter";
            sol::table biomeTable = biomeRef.second.as<sol::table>();

            // Environment Properties for Voronoi Diagram
            auto environment = biomeTable.get<sol::optional<sol::table>>("environment");
            if (!environment) throw "biome definitions require an environment table";

            auto temperature = environment->get_or<float>("temperature", -2);
            auto humidity    = environment->get_or<float>("humidity",    -2);
            auto roughness   = environment->get_or<float>("roughness",   -2);

            if (temperature < -1 || temperature > 1)
                throw "environment.temperature property is out of range or missing";
            if (humidity < 0 || humidity > 1)
                throw "environment.humidity property is out of range or missing";
            if (roughness < 0 || roughness > 1)
                throw "environment.roughness property is out of range or missing";

            // Blocks to form the environment out of
            auto blocksList = biomeTable.get<sol::optional<sol::table>>("blocks");
            if (!blocksList) throw identifier + "biome definitions require a blocks table";

            auto bTop  = (*blocksList).get<sol::optional<std::string>>("top");
            auto bSoil = (*blocksList).get<sol::optional<std::string>>("soil");
            auto bRock = (*blocksList).get<sol::optional<std::string>>("rock");

            if (!bTop)  throw identifier + "blocks.top property is missing";
            if (!bSoil) throw identifier + "blocks.soil property is missing";
            if (!bRock) throw identifier + "blocks.rock property is missing";

            // Get biome tint
            auto biomeTint = biomeTable.get < sol::optional < std::string >> ("biome_tint");
            if (!biomeTint) throw identifier + "biome definitions require a biome_tint";

            // Parse Noise params
            std::vector<noise::module::Module*> modules;
            parseNoise(modules, biomeTable.get<sol::table>("noise"));

            // Create biome definition
            BiomeDef* biomeDef = new BiomeDef(
                    identifier, biomes.size(),
                    temperature, humidity, roughness,
                    defs.blockFromStr(*bTop).index,
                    defs.blockFromStr(*bSoil).index,
                    defs.blockFromStr(*bRock).index,
                    modules,
                    glm::vec3(Util::hexToColorVec((*biomeTint)))
            );

            // Add biome to biomes
            biomes.registerBiome(biomeDef);
        }
    }

    static void server(sol::table& core, ServerDefs& defs) {
        registerBiomes(core.get<sol::table>("registered_biomes"), defs.defs, defs.biomes);
        defs.biomes.generateVoronoi();
    }

    static void client(sol::table& core, LocalDefs& defs) {
        registerBiomes(core.get<sol::table>("registered_biomes"), defs.defs, defs.biomes);
        defs.biomes.generateVoronoi();
    }
};
