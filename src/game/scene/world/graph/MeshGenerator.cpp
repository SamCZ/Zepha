//
// Created by aurailus on 01/12/18.
//

#include "MeshGenerator.h"

MeshGenerator::MeshGenerator() {
    indOffset = 0;
}

BlockDef& blockData(int ind, BlockChunk &chunk, BlockAtlas& atlas) {
    return atlas.getBlock(chunk.getBlock(ind));
}

BlockDef& blockData(glm::vec3 &pos, BlockChunk &chunk, BlockAtlas &atlas) {
    return atlas.getBlock(chunk.getBlock(&pos));
}

bool faceOcculudedAt(glm::vec3 &pos, BlockChunk &chunk, BlockAtlas &atlas, std::vector<bool> &bools) {
    if (pos.x < 0 || pos.x > 15 || pos.y < 0 || pos.y > 15 || pos.z < 0 || pos.z > 15) {

        if (pos.x == -1) return bools[ 256 + (int)pos.y * 16 + (int)pos.z];
        if (pos.x == 16) return bools[     + (int)pos.y * 16 + (int)pos.z];

        if (pos.y == -1) return bools[ 768 + (int)pos.x * 16 + (int)pos.z];
        if (pos.y == 16) return bools[ 512 + (int)pos.x * 16 + (int)pos.z];

        if (pos.z == -1) return bools[1280 + (int)pos.y * 16 + (int)pos.x];
        if (pos.z == 16) return bools[1024 + (int)pos.y * 16 + (int)pos.x];

        return false;
    }
    return blockData(pos, chunk, atlas).isCulling();
}

void MeshGenerator::build(const std::shared_ptr<BlockChunk> &chunk, BlockAtlas &atlas, std::vector<bool> &adjacents,
        std::vector<float> &vertices, std::vector<unsigned int> &indices) {

    Timer t("Mesh Generation");

    vertices.reserve(300000);
    indices.reserve(50000);

    glm::vec3 off;
    glm::vec3 check;

    for (int i = 0; i < 4096; i++) {
        if (blockData(i, *chunk, atlas).getModel().visible) {

            VecUtils::indAssignVec(i, off);
            BlockModel& model = blockData(i, *chunk, atlas).getModel();

            check.x = off.x - 1; check.y = off.y; check.z = off.z;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(off, vertices, indices, model.leftFaces);

            check.x = off.x + 1; check.y = off.y; check.z = off.z;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(off, vertices, indices, model.rightFaces);

            check.x = off.x; check.y = off.y - 1; check.z = off.z;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(off, vertices, indices, model.bottomFaces);

            check.x = off.x; check.y = off.y + 1; check.z = off.z;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(off, vertices, indices, model.topFaces);

            check.x = off.x; check.y = off.y; check.z = off.z - 1;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(off, vertices, indices, model.backFaces);

            check.x = off.x; check.y = off.y; check.z = off.z + 1;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(off, vertices, indices, model.frontFaces);

            addFaces(off, vertices, indices, model.noCulledFaces);
        }
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();
}

void MeshGenerator::addFaces(glm::vec3 &offset, vector<float> &vertices, vector<unsigned int> &indices, vector<MeshPart> &meshParts) {
    for (const MeshPart& mp : meshParts) {

        for (const MeshVertex &vertex : mp.vertices) {

            vertices.push_back(vertex.pos.x + offset.x);
            vertices.push_back(vertex.pos.y + offset.y);
            vertices.push_back(vertex.pos.z + offset.z);

            vertices.push_back(1);

            vertices.push_back(vertex.tex.x);
            vertices.push_back(vertex.tex.y);
            vertices.push_back(0);
            vertices.push_back(0);

            vertices.push_back(vertex.nml.x);
            vertices.push_back(vertex.nml.y);
            vertices.push_back(vertex.nml.z);
        }

        for (unsigned int index : mp.indices) {
            indices.push_back(indOffset + index);
        }

        indOffset += mp.vertices.size();
    }
}

void MeshGenerator::cleanup() {

}

MeshGenerator::~MeshGenerator() {
    cleanup();
}