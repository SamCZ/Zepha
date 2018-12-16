//
// Created by aurailus on 15/12/18.
//

#ifndef GLPROJECT_MESHCHUNK_H
#define GLPROJECT_MESHCHUNK_H


#include "BlockChunk.h"
#include "../engine/Entity.h"
#include "../mesh/MeshGenerator.h"

class MeshChunk : public Entity {
public:
    MeshChunk();
    explicit MeshChunk(BlockChunk* blockChunk);

    void build(BlockAtlas* atlas);

    bool isReady();
private:
    bool ready;
    BlockChunk* blockChunk;
};


#endif //GLPROJECT_MESHCHUNK_H
