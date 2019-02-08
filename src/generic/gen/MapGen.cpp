//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"

void MapGen::init(unsigned int seed) {
}

BlockChunk* MapGen::generate(glm::vec3 pos) {
    MapGenJob j(pos);

    getElevation(j);
    getBiome(j);
    fillBlocks(j);

    return new BlockChunk(j.blocks);
}

void MapGen::getElevation(MapGen::MapGenJob &j) {
    glm::vec3* localPos;
    glm::vec3 globalPos;

    for (int i = 0; i < 4096; i++) {
        localPos = ArrayTrans3D::indToVec(i);
        globalPos = glm::vec3(j.pos.x * 16 + localPos->x, j.pos.y * 16 + localPos->y, j.pos.z * 16 + localPos->z);

        int val = (int)floor(p.noise(globalPos.x / 16, 0, globalPos.z / 16) * 32);
        val /= (int)floor(p.noise(globalPos.x / 32, 0, globalPos.z / 32) * 16);

        val -= (int)globalPos.y;

        j.elevation[i] = val;
    }
}

void MapGen::getBiome(MapGen::MapGenJob &j) {
    //TODO: Biome Voronoi calculation
}

void MapGen::fillBlocks(MapGen::MapGenJob &j) {
    glm::vec3* localPos;
    glm::vec3 globalPos;

    for (int i = 0; i < 4096; i++) {
        int ev = j.elevation[i];
        (*(j.blocks))[i] = ev < 0 ? 0 : ev < 1 ? 1 : ev < 2 ? 2 : 3;
    }
}
