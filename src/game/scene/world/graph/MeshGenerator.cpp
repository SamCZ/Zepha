//
// Created by aurailus on 01/12/18.
//

#include "MeshGenerator.h"

MeshGenerator::MeshGenerator() {
    indOffset = 0;
}

LocalBlockDef& blockData(int ind, BlockChunk &chunk, LocalBlockAtlas& atlas) {
    return atlas.fromIndex(chunk.getBlock(ind));
}

LocalBlockDef& blockData(glm::vec3 &pos, BlockChunk &chunk, LocalBlockAtlas &atlas) {
    return atlas.fromIndex(chunk.getBlock(&pos));
}

bool faceOcculudedAt(glm::vec3 &pos, BlockChunk &chunk, LocalBlockAtlas &atlas, std::vector<bool> &bools) {
    auto off = TransPos::CHUNK_SIZE*TransPos::CHUNK_SIZE; //CHUNK_SIZE ^ 2
    if (pos.x < 0 || pos.x >= TransPos::CHUNK_SIZE || pos.y < 0 || pos.y >= TransPos::CHUNK_SIZE || pos.z < 0 || pos.z >= TransPos::CHUNK_SIZE) {

        if (pos.x == -1) return bools[off + (int)pos.y * TransPos::CHUNK_SIZE + (int)pos.z];
        if (pos.x == TransPos::CHUNK_SIZE) return bools[    + (int)pos.y * TransPos::CHUNK_SIZE + (int)pos.z];

        if (pos.y == -1) return bools[off*3 + (int)pos.x * TransPos::CHUNK_SIZE + (int)pos.z];
        if (pos.y == TransPos::CHUNK_SIZE) return bools[off*2 + (int)pos.x * TransPos::CHUNK_SIZE + (int)pos.z];

        if (pos.z == -1) return bools[off*5 + (int)pos.y * TransPos::CHUNK_SIZE + (int)pos.x];
        if (pos.z == TransPos::CHUNK_SIZE) return bools[off*4 + (int)pos.y * TransPos::CHUNK_SIZE + (int)pos.x];

        return false;
    }
    return blockData(pos, chunk, atlas).isCulling();
}

void MeshGenerator::build(const std::shared_ptr<BlockChunk> &chunk, LocalBlockAtlas &atlas, std::vector<bool> &adjacents,
        std::vector<Vertex> &vertices, std::vector<unsigned int> &indices) {

    Timer t("Mesh Generation");

    vertices.reserve(300000);
    indices.reserve(50000);

    glm::vec3 off;
    glm::vec3 vis;
    glm::vec3 check;

    for (int i = 0; i < (int)pow(TransPos::CHUNK_SIZE, 3); i++) {
        if (blockData(i, *chunk, atlas).getModel().visible) {

            VecUtils::indAssignVec(i, off);
            vis = off;
//            if (chunk->getBlock(i) >= 6 && chunk->fromIndex(i) <= 10) {
//                vis += glm::vec3((((double)rand() / RAND_MAX) - 0.5f) / 3.f, 0, (((double)rand() / RAND_MAX) - 0.5f) / 3.f);
//            }

            //TODO: Better way to do this, surely

            LocalBlockModel& model = blockData(i, *chunk, atlas).getModel();

            check.x = off.x - 1; check.y = off.y; check.z = off.z;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(vis, vertices, indices, model.parts[XNEG]);

            check.x = off.x + 1; check.y = off.y; check.z = off.z;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(vis, vertices, indices, model.parts[XPOS]);

            check.x = off.x; check.y = off.y - 1; check.z = off.z;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(vis, vertices, indices, model.parts[YNEG]);

            check.x = off.x; check.y = off.y + 1; check.z = off.z;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(vis, vertices, indices, model.parts[YPOS]);

            check.x = off.x; check.y = off.y; check.z = off.z - 1;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(vis, vertices, indices, model.parts[ZNEG]);

            check.x = off.x; check.y = off.y; check.z = off.z + 1;
            if (!faceOcculudedAt(check, *chunk, atlas, adjacents))
                addFaces(vis, vertices, indices, model.parts[ZPOS]);

            addFaces(vis, vertices, indices, model.parts[NO_CULL]);
        }
    }

    vertices.shrink_to_fit();
    indices.shrink_to_fit();
}

void MeshGenerator::addFaces(glm::vec3 &offset, vector<Vertex> &vertices, vector<unsigned int> &indices, vector<LocalMeshPart> &meshParts) {
    for (const LocalMeshPart& mp : meshParts) {

        for (const MeshVertex &vertex : mp.vertices) {
            vertices.push_back({{vertex.pos + offset}, 1, {vertex.tex.x, vertex.tex.y, 0, 0}, vertex.nml});
        }

        for (unsigned int index : mp.indices) {
            indices.push_back(indOffset + index);
        }

        indOffset += mp.vertices.size();
    }
}