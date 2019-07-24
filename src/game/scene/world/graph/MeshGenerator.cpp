//
// Created by aurailus on 01/12/18.
//

#include <thread>
#include "MeshGenerator.h"

MeshGenerator::MeshGenerator(MeshDetails* meshDetails, LocalBlockAtlas& atlas,
                             std::shared_ptr<BlockChunk> chunk, std::array<std::shared_ptr<BlockChunk>, 6> adjacent,
                             std::array<NoiseSample, 3>& blockOffsets) :
    meshDetails(meshDetails),
    atlas(atlas),
    chunk(chunk),
    adjacent(adjacent) {

    meshDetails->vertices.reserve(10000);
    meshDetails->indices.reserve(10000);

    Timer t("ChunkMesh Generation");

    glm::vec3 off;
    glm::vec3 vis;
    glm::vec3 check;

    auto end = (int)pow(TransPos::CHUNK_SIZE, 3);
    for (int i = 0; i < end; i++) {
        LocalBlockModel& model = atlas.fromIndex(chunk->getBlock(i)).model;

        if (model.visible) {
            VecUtils::indAssignVec(i, off);

            vis = off;

            for (auto& mod : model.meshMods) {
                switch (mod.first) {
                    default: break;

                    case MeshMod::OFFSET_X: {
                        vis.x += blockOffsets[0].get(off) * mod.second;
                        break;
                    }
                    case MeshMod::OFFSET_Y: {
                        vis.y += blockOffsets[1].get(off) * mod.second;
                        break;
                    }
                    case MeshMod::OFFSET_Z: {
                        vis.z += blockOffsets[2].get(off) * mod.second;
                        break;
                    }
                }
            }

            if (model.visible) {
                check = off; check.x -= 1;
                if (!getBlockAt(check).culls) addFaces(vis, model.parts[XNEG]);
                check = off; check.x += 1;
                if (!getBlockAt(check).culls) addFaces(vis, model.parts[XPOS]);
                check = off; check.y -= 1;
                if (!getBlockAt(check).culls) addFaces(vis, model.parts[YNEG]);
                check = off; check.y += 1;
                if (!getBlockAt(check).culls) addFaces(vis, model.parts[YPOS]);
                check = off; check.z -= 1;
                if (!getBlockAt(check).culls) addFaces(vis, model.parts[ZNEG]);
                check = off; check.z += 1;
                if (!getBlockAt(check).culls) addFaces(vis, model.parts[ZPOS]);

                addFaces(vis, model.parts[NO_CULL]);
            }
        }
    }

    meshDetails->vertices.shrink_to_fit();
    meshDetails->indices.shrink_to_fit();
}

LocalBlockDef& MeshGenerator::getBlockAt(const glm::vec3 &pos) {
    if (pos.x < 0 || pos.x >= TransPos::CHUNK_SIZE || pos.y < 0 || pos.y >= TransPos::CHUNK_SIZE || pos.z < 0 || pos.z >= TransPos::CHUNK_SIZE) {

        if (pos.x == TransPos::CHUNK_SIZE) return atlas.fromIndex(adjacent[0]->getBlock(pos - glm::vec3(TransPos::CHUNK_SIZE, 0, 0)));
        if (pos.x == -1) return atlas.fromIndex(adjacent[1]->getBlock(pos + glm::vec3(TransPos::CHUNK_SIZE, 0, 0)));

        if (pos.y == TransPos::CHUNK_SIZE) return atlas.fromIndex(adjacent[2]->getBlock(pos - glm::vec3(0, TransPos::CHUNK_SIZE, 0)));
        if (pos.y == -1) return atlas.fromIndex(adjacent[3]->getBlock(pos + glm::vec3(0, TransPos::CHUNK_SIZE, 0)));

        if (pos.z == TransPos::CHUNK_SIZE) return atlas.fromIndex(adjacent[4]->getBlock(pos - glm::vec3(0, 0, TransPos::CHUNK_SIZE)));
        if (pos.z == -1) return atlas.fromIndex(adjacent[5]->getBlock(pos + glm::vec3(0, 0, TransPos::CHUNK_SIZE)));
    }

    return atlas.fromIndex(chunk->getBlock(pos));
}

void MeshGenerator::addFaces(const glm::vec3 &offset, const vector<LocalMeshPart> &meshParts) {
    for (const LocalMeshPart& mp : meshParts) {

        glm::vec3 modData = {};

        switch (mp.shaderMod) {
            default: break;

            case ShaderMod::ROTATE_X:
            case ShaderMod::ROTATE_Y:
            case ShaderMod::ROTATE_Z:
            case ShaderMod::SWAY_ATTACHED:
            case ShaderMod::SWAY_FULL_BLOCK: {
                modData = {Util::packFloat((offset - 8.f) / 8), mp.modValue, 0};
                break;
            }
        }

        for (const MeshVertex &vertex : mp.vertices) {
            meshDetails->vertices.push_back({
                   vertex.pos + offset,
                   vertex.tex,
                   Util::packFloat(vertex.nml),
                   static_cast<float>(mp.shaderMod),
                   modData
            });
        }

        for (unsigned int index : mp.indices) {
            meshDetails->indices.push_back(indOffset + index);
        }

        indOffset += mp.vertices.size();
    }
}