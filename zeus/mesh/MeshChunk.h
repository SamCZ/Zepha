//
// Created by aurailus on 15/12/18.
//

#ifndef GLPROJECT_MESHCHUNK_H
#define GLPROJECT_MESHCHUNK_H


#include "../blocks/BlockChunk.h"
#include "../engine/Entity.h"
#include "MeshGenerator.h"

class MeshChunk : public Entity {
public:
    MeshChunk();

    void build(std::vector<float> *vertices, std::vector<unsigned int> *indices);

    bool isReady();
private:
    bool ready;
};


#endif //GLPROJECT_MESHCHUNK_H
