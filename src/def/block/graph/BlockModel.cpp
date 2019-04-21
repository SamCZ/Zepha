//
// Created by aurailus on 04/12/18.
//

#include "BlockModel.h"

//TODO: Refactor to not take lua properties to allow for unit tests
BlockModel BlockModel::create(sol::table model, std::vector<std::string> textures, TextureAtlas& atlas, bool visible, bool culls) {
    BlockModel blockModel;

    try {
        blockModel.culls = culls;
        blockModel.visible = visible;

        model.for_each([&](sol::object key, sol::object value) {
            if (!value.is<sol::table>()) throw "Meshpart is not a table";
            sol::table meshPart = value.as<sol::table>();

            //This value is sanitized later
            std::string face = meshPart.get_or<std::string>("face", "nocull");

            //TODO: Validate that tex is greater than 0
            int tex = (int)meshPart.get_or<float>("tex", 1);

            auto oPoints = meshPart.get<sol::optional<sol::table>>("points");
            if (!oPoints) throw "Meshpart is missing points field";
            sol::table points = *oPoints;

            int texturesLength = textures.size();

            std::vector<MeshVertex> vertices;
            std::vector<unsigned int> indices;

            if (points.size() % 20 != 0) throw "Points array is not well formed. (Not a multiple of 20 values)";

            for (int i = 1; i <= points.size()/5; i++) {
                int offset = (i - 1) * 5 + 1;

                glm::vec3 pos(points[offset], points[offset + 1], points[offset + 2]);
                glm::vec2 tex(points[offset + 3], points[offset + 4]);

                vertices.push_back({pos, {0, 0, 0}, tex, {0, 0}});
            }

            int ind = 0;
            for (int i = 1; i <= points.size() / 20; i++) {
                indices.push_back(ind);
                indices.push_back(ind + 1);
                indices.push_back(ind + 2);
                indices.push_back(ind + 2);
                indices.push_back(ind + 3);
                indices.push_back(ind);
                ind += 4;
            }

            MeshPart partObject(std::move(vertices), std::move(indices),
                    textures[std::min(tex - 1, (int)textures.size() - 1)], atlas);

            if      (face == "top")    blockModel.topFaces.push_back(partObject);
            else if (face == "bottom") blockModel.bottomFaces.push_back(partObject);
            else if (face == "left")   blockModel.leftFaces.push_back(partObject);
            else if (face == "right")  blockModel.rightFaces.push_back(partObject);
            else if (face == "front")  blockModel.frontFaces.push_back(partObject);
            else if (face == "back")   blockModel.backFaces.push_back(partObject);
            else if (face == "nocull") blockModel.noCulledFaces.push_back(partObject);
            else throw "Face value is not one of 'top', 'bottom', 'left', 'right', 'front', 'back', 'nocull'.";
        });
    }
    catch (const std::string& e) {
        std::cerr << "Exception on BlockModel constructor: " << e << std::endl;
    }

    return std::move(blockModel);
}