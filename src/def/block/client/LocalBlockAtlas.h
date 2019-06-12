//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_BLOCKATLAS_H
#define GLPROJECT_BLOCKATLAS_H


#include <vector>
#include "LocalBlockDef.h"
#include "../../texture/TextureAtlas.h"
#include "../BlockAtlas.h"

class LocalBlockAtlas : public BlockAtlas {
public:
    LocalBlockAtlas();

    void registerBlock(LocalBlockDef def);

    int definitionsSize() override;

    LocalBlockDef& fromIndex(int id) override;
    LocalBlockDef& fromIdentifier(std::string identifier) override;
private:
    const static int AIR = 0;

    std::vector<LocalBlockDef> definitions;
    std::unordered_map<std::string, int> identifierIndexTable;
};


#endif //GLPROJECT_BLOCKATLAS_H
