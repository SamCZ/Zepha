//
// Created by aurailus on 13/08/19.
//

#pragma once

#include <set>
#include <vector>
#include "MeshPart.h"
#include "../../util/Dir.h"
#include <algorithm>
#include <limits>

struct BlockModel {
    std::array<std::vector<MeshPart>, 7> parts;
    std::vector<std::pair<MeshMod, float>> meshMods;
    std::set<std::shared_ptr<AtlasRef>> textureRefs;

    bool culls = false;
    bool visible = false;

    static BlockModel createCube(const std::vector<std::shared_ptr<AtlasRef>>& textureRefs) {
        BlockModel blockModel;

        std::vector<BlockModelVertex> vertices {};
        std::vector<unsigned int> indices {};

        for (auto& ref : textureRefs) blockModel.textureRefs.insert(ref);

        //Left Face
        vertices = {
                {glm::vec3{0, 0, 0}, glm::vec3{}, glm::vec2{0, 1}, glm::vec2{}},
                {glm::vec3{0, 0, 1}, glm::vec3{}, glm::vec2{1, 1}, glm::vec2{}},
                {glm::vec3{0, 1, 1}, glm::vec3{}, glm::vec2{1, 0}, glm::vec2{}},
                {glm::vec3{0, 1, 0}, glm::vec3{}, glm::vec2{0, 0}, glm::vec2{}}};
        indices = {0, 1, 2, 2, 3, 0};
        MeshPart leftMeshPart(vertices, indices, (textureRefs.empty()) ? nullptr : textureRefs[(std::min)(static_cast<int>(textureRefs.size() - 1), 2)], false);
        blockModel.parts[static_cast<int>(Dir::LEFT)].push_back(leftMeshPart);

        //Right Face
        vertices = {
                {glm::vec3{1, 1, 1}, glm::vec3{}, glm::vec2{1, 0}, glm::vec2{}},
                {glm::vec3{1, 0, 1}, glm::vec3{}, glm::vec2{1, 1}, glm::vec2{}},
                {glm::vec3{1, 0, 0}, glm::vec3{}, glm::vec2{0, 1}, glm::vec2{}},
                {glm::vec3{1, 1, 0}, glm::vec3{}, glm::vec2{0, 0}, glm::vec2{}}};
        indices = {0, 1, 2, 2, 3, 0};
        MeshPart rightMeshPart(vertices, indices, (textureRefs.empty()) ? nullptr : textureRefs[(std::min)(static_cast<int>(textureRefs.size() - 1), 3)], false);
        blockModel.parts[static_cast<int>(Dir::RIGHT)].push_back(rightMeshPart);

        //Top Face
        vertices = {
                {glm::vec3{0, 1, 0}, glm::vec3{}, glm::vec2{0, 0}, glm::vec2{}},
                {glm::vec3{0, 1, 1}, glm::vec3{}, glm::vec2{0, 1}, glm::vec2{}},
                {glm::vec3{1, 1, 1}, glm::vec3{}, glm::vec2{1, 1}, glm::vec2{}},
                {glm::vec3{1, 1, 0}, glm::vec3{}, glm::vec2{1, 0}, glm::vec2{}}};
        indices = {0, 1, 2, 2, 3, 0};
        MeshPart topMeshPart(vertices, indices, (textureRefs.empty()) ? nullptr : textureRefs[(std::min)(static_cast<int>(textureRefs.size() - 1), 0)], false);
        blockModel.parts[static_cast<int>(Dir::TOP)].push_back(topMeshPart);

        //Bottom Face
        vertices = {
                {glm::vec3{0, 0, 0}, glm::vec3{}, glm::vec2{0, 0}, glm::vec2{}},
                {glm::vec3{1, 0, 0}, glm::vec3{}, glm::vec2{1, 0}, glm::vec2{}},
                {glm::vec3{1, 0, 1}, glm::vec3{}, glm::vec2{1, 1}, glm::vec2{}},
                {glm::vec3{0, 0, 1}, glm::vec3{}, glm::vec2{0, 1}, glm::vec2{}}};
        indices = {0, 1, 2, 2, 3, 0};
        MeshPart bottomMeshPart(vertices, indices, (textureRefs.empty()) ? nullptr : textureRefs[(std::min)(static_cast<int>(textureRefs.size() - 1), 1)], false);
        blockModel.parts[static_cast<int>(Dir::BOTTOM)].push_back(bottomMeshPart);

        //Front Face
        vertices = {
                {glm::vec3{0, 0, 1}, glm::vec3{}, glm::vec2{0, 1}, glm::vec2{}},
                {glm::vec3{1, 0, 1}, glm::vec3{}, glm::vec2{1, 1}, glm::vec2{}},
                {glm::vec3{1, 1, 1}, glm::vec3{}, glm::vec2{1, 0}, glm::vec2{}},
                {glm::vec3{0, 1, 1}, glm::vec3{}, glm::vec2{0, 0}, glm::vec2{}}};
        indices = {0, 1, 2, 2, 3, 0};
        MeshPart frontMeshPart(vertices, indices, (textureRefs.empty()) ? nullptr : textureRefs[(std::min)(static_cast<int>(textureRefs.size() - 1), 4)], false);
        blockModel.parts[static_cast<int>(Dir::FRONT)].push_back(frontMeshPart);

        //Back Face
        vertices = {
                {glm::vec3{0, 0, 0}, glm::vec3{}, glm::vec2{0, 1}, glm::vec2{}},
                {glm::vec3{0, 1, 0}, glm::vec3{}, glm::vec2{0, 0}, glm::vec2{}},
                {glm::vec3{1, 1, 0}, glm::vec3{}, glm::vec2{1, 0}, glm::vec2{}},
                {glm::vec3{1, 0, 0}, glm::vec3{}, glm::vec2{1, 1}, glm::vec2{}}};
        indices = {0, 1, 2, 2, 3, 0};
        MeshPart backMeshPart(vertices, indices, (textureRefs.empty()) ? nullptr : textureRefs[(std::min)(static_cast<int>(textureRefs.size() - 1), 5)], false);
        blockModel.parts[static_cast<int>(Dir::BACK)].push_back(backMeshPart);

        return blockModel;
    };
};