//
// Created by aurailus on 01/10/19.
//

#pragma once

#include "Dimension.h"

class ServerDimension : public Dimension {
public:
    ServerDimension() = default;

    void setChunk(std::shared_ptr<BlockChunk> chunk) override;
    bool setBlock(glm::vec3 pos, unsigned int block) override;

    unsigned long long getMapBlockIntegrity(glm::vec3 mapBlock);

private:
    std::unordered_map<glm::vec3, unsigned long long, Vec::compareFunc> mapBlockIntegrity {};
};

