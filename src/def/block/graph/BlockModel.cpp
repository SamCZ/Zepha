//
// Created by aurailus on 04/12/18.
//

#include "BlockModel.h"

//TODO: Refactor to not take lua properties to allow for unit tests
BlockModel BlockModel::from_lua_def(sol::table model, sol::table textures, TextureAtlas& atlas, bool visible, bool culls) {
    BlockModel blockModel;

    blockModel.culls = culls;
    blockModel.visible = visible;

    for (auto meshPartPair : model) {
        auto meshPart = (sol::table)meshPartPair.second;

        std::string face = meshPart.get_or<std::string>("face", "nocull");
        int tex = (int)meshPart.get_or<float>("tex", 0);

        auto pointsOptional = meshPart.get<sol::optional<sol::table>>("points");
        if (!pointsOptional) break;
        sol::table points = *pointsOptional;

        int texturesLength = textures.size();

        std::string texture = textures.get_or<std::string>(std::min(tex, texturesLength), "_missing");

        std::vector<MeshVertex> vertices;
        std::vector<unsigned int> indices;

        for (int i = 1; i <= points.size()/5; i++) {
            int o = (i-1) * 5 + 1;

            glm::vec3 pos((float)points[o], (float)points[o+1], (float)points[o+2]);
            glm::vec2 tex((float)points[o+3], (float)points[o+4]);

            vertices.push_back({pos, {0, 0, 0}, tex, {0, 0}});
        }

        int ind = 0;
        for (int i = 1; i <= points.size()/20; i++) {
            indices.push_back(ind);
            indices.push_back(ind + 1);
            indices.push_back(ind + 2);
            indices.push_back(ind + 2);
            indices.push_back(ind + 3);
            indices.push_back(ind);
            ind += 4;
        }

        auto mp = MeshPart(std::move(vertices), std::move(indices), texture, atlas);

        if      (face == "top")    blockModel.topFaces.push_back(mp);
        else if (face == "bottom") blockModel.bottomFaces.push_back(mp);
        else if (face == "left")   blockModel.leftFaces.push_back(mp);
        else if (face == "right")  blockModel.rightFaces.push_back(mp);
        else if (face == "front")  blockModel.frontFaces.push_back(mp);
        else if (face == "back")   blockModel.backFaces.push_back(mp);
        else if (face == "nocull") blockModel.noCulledFaces.push_back(mp);
    }

    return std::move(blockModel);
}