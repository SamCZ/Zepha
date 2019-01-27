//
// Created by aurailus on 14/12/18.
//

#ifndef GLPROJECT_BLOCKCHUNK_H
#define GLPROJECT_BLOCKCHUNK_H

#include <vec3.hpp>
#include <vector>
#include <iostream>
#include "../helpers/ArrayTrans3D.h"
#include "../network/Packet.h"

class BlockChunk {
public:
    BlockChunk();
    explicit BlockChunk(std::vector<int>* blocks);

    bool adjacent[6] = {false, false, false, false, false, false};
    bool allAdjacentsExist();

    int getBlock(int ind);
    int getBlock(glm::vec3* pos);
    int getBlock(int x, int y, int z);

    bool setBlock(glm::vec3* pos, int ind);

    std::vector<int>* rleEncode();
    void rleDecode(std::vector<int>* blocksRle);

    std::string serialize();
    void deserialize(std::string gzip);
private:
    std::vector<int>* blocks;
};

#endif //GLPROJECT_BLOCKCHUNK_H