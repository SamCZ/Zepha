//
// Created by aurailus on 2020-07-06.
//

#include "BlockModel.h"

#include "util/Vec.h"

BlockModel
BlockModel::createCube(std::vector<std::shared_ptr<AtlasRef>> textureRefs,
    std::vector<unsigned int> blendInds, std::vector<std::shared_ptr<AtlasRef>> blendMaskRefs) {

    BlockModel blockModel;
    blockModel.visible = true;
    blockModel.culls = true;

    std::vector<BlockModelVertex> vertices {};
    std::vector<unsigned int> indices {};
    unsigned int accessInd;

    for (auto& ref : textureRefs) if (ref != nullptr) blockModel.textureRefs.insert(ref);

    if (textureRefs.empty()) textureRefs.emplace_back(nullptr);
    if (blendInds.empty()) blendInds.emplace_back(0);
    if (blendMaskRefs.empty()) blendMaskRefs.emplace_back(nullptr);

    //Left Face
    vertices = {
            {{0, 0, 0}, {}, {0, 1}, {0, 1}},
            {{0, 0, 1}, {}, {1, 1}, {1, 1}},
            {{0, 1, 1}, {}, {1, 0}, {1, 0}},
            {{0, 1, 0}, {}, {0, 0}, {0, 0}}};
    indices = {0, 1, 2, 2, 3, 0};
    accessInd = (std::max)(0, (std::min)(static_cast<int>(textureRefs.size() - 1), 2));
    MeshPart leftMeshPart(vertices, indices, textureRefs[accessInd], blendInds[accessInd], blendMaskRefs[accessInd]);
    blockModel.parts[static_cast<int>(EVec::LEFT)].push_back(leftMeshPart);

    //Right Face
    vertices = {
            {{1, 1, 1}, {}, {1, 0}, {1, 0}},
            {{1, 0, 1}, {}, {1, 1}, {1, 1}},
            {{1, 0, 0}, {}, {0, 1}, {0, 1}},
            {{1, 1, 0}, {}, {0, 0}, {0, 0}}};
    indices = {0, 1, 2, 2, 3, 0};
    accessInd = (std::max)(0, (std::min)(static_cast<int>(textureRefs.size() - 1), 3));
    MeshPart rightMeshPart(vertices, indices, textureRefs[accessInd], blendInds[accessInd], blendMaskRefs[accessInd]);
    blockModel.parts[static_cast<int>(EVec::RIGHT)].push_back(rightMeshPart);

    //Top Face
    vertices = {
            {{0, 1, 0}, {}, {0, 0}, {0, 0}},
            {{0, 1, 1}, {}, {0, 1}, {0, 1}},
            {{1, 1, 1}, {}, {1, 1}, {1, 1}},
            {{1, 1, 0}, {}, {1, 0}, {1, 0}}};
    indices = {0, 1, 2, 2, 3, 0};
    accessInd = (std::max)(0, (std::min)(static_cast<int>(textureRefs.size() - 1), 0));
    MeshPart topMeshPart(vertices, indices, textureRefs[accessInd], blendInds[accessInd], blendMaskRefs[accessInd]);
    blockModel.parts[static_cast<int>(EVec::TOP)].push_back(topMeshPart);

    //Bottom Face
    vertices = {
            {{0, 0, 0}, {}, {0, 0}, {0, 0}},
            {{1, 0, 0}, {}, {1, 0}, {1, 0}},
            {{1, 0, 1}, {}, {1, 1}, {1, 1}},
            {{0, 0, 1}, {}, {0, 1}, {0, 1}}};
    indices = {0, 1, 2, 2, 3, 0};
    accessInd = (std::max)(0, (std::min)(static_cast<int>(textureRefs.size() - 1), 1));
    MeshPart bottomMeshPart(vertices, indices, textureRefs[accessInd], blendInds[accessInd], blendMaskRefs[accessInd]);
    blockModel.parts[static_cast<int>(EVec::BOTTOM)].push_back(bottomMeshPart);

    //Front Face
    vertices = {
            {{0, 0, 1}, {}, {0, 1}, {0, 1}},
            {{1, 0, 1}, {}, {1, 1}, {1, 1}},
            {{1, 1, 1}, {}, {1, 0}, {1, 0}},
            {{0, 1, 1}, {}, {0, 0}, {0, 0}}};
    indices = {0, 1, 2, 2, 3, 0};
    accessInd = (std::max)(0, (std::min)(static_cast<int>(textureRefs.size() - 1), 4));
    MeshPart frontMeshPart(vertices, indices, textureRefs[accessInd], blendInds[accessInd], blendMaskRefs[accessInd]);
    blockModel.parts[static_cast<int>(EVec::FRONT)].push_back(frontMeshPart);

    //Back Face
    vertices = {
            {{0, 0, 0}, {}, {0, 1}, {0, 1}},
            {{0, 1, 0}, {}, {0, 0}, {0, 0}},
            {{1, 1, 0}, {}, {1, 0}, {1, 0}},
            {{1, 0, 0}, {}, {1, 1}, {1, 1}}};
    indices = {0, 1, 2, 2, 3, 0};
    accessInd = (std::max)(0, (std::min)(static_cast<int>(textureRefs.size() - 1), 5));
    MeshPart backMeshPart(vertices, indices, textureRefs[accessInd], blendInds[accessInd], blendMaskRefs[accessInd]);
    blockModel.parts[static_cast<int>(EVec::BACK)].push_back(backMeshPart);

    return blockModel;
}

