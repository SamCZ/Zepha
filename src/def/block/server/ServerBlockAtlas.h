//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_SERVERBLOCKATLAS_H
#define ZEUS_SERVERBLOCKATLAS_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "ServerBlockDef.h"
#include "../BlockAtlas.h"

class ServerBlockAtlas : public BlockAtlas {
public:
    ServerBlockAtlas();

    void registerBlock(ServerBlockDef def);

    int definitionsSize() override;

    ServerBlockDef& fromIndex(unsigned int id) override;
    ServerBlockDef& fromIdentifier(std::string identifier) override;
private:
    std::vector<ServerBlockDef> definitions;
    std::unordered_map<std::string, int> identifierIndexTable;
};

#endif //ZEUS_SERVERBLOCKATLAS_H
