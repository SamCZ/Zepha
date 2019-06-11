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

class ServerBlockAtlas {
public:
    ServerBlockAtlas();

    void registerBlock(ServerBlockDef def);

    ServerBlockDef& fromIndex(int id);
    ServerBlockDef& fromIdentifier(std::string identifier);
private:
    const static int AIR = 0;

    std::vector<ServerBlockDef> definitions;
    std::unordered_map<std::string, int> identifierIndexTable;
};

#endif //ZEUS_SERVERBLOCKATLAS_H
