//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_BLOCKATLAS_H
#define GLPROJECT_BLOCKATLAS_H


#include <vector>
#include "LocalBlockDef.h"
#include "../../texture/TextureAtlas.h"

class LocalBlockAtlas {
public:
    LocalBlockAtlas();

    void registerBlock(LocalBlockDef def);

    LocalBlockDef& fromIndex(int id);
    LocalBlockDef& fromIdentifier(std::string identifier);
private:
    const static int AIR = 0;

    std::vector<LocalBlockDef> definitions;
    std::unordered_map<std::string, int> identifierIndexTable;
};


#endif //GLPROJECT_BLOCKATLAS_H
