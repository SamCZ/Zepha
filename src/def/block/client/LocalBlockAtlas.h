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

    void setIdentifiers(std::vector<std::string>& identifiers);
    void registerBlock(LocalBlockDef def);

    int definitionsSize() override {return 0;};

    LocalBlockDef& fromIndex(unsigned int id) override;
    LocalBlockDef& fromIdentifier(std::string identifier) override;
private:
    std::vector<LocalBlockDef> definitions {2};
    std::unordered_map<std::string, int> identifierIndexTable;
};


#endif //GLPROJECT_BLOCKATLAS_H
