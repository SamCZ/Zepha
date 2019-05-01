//
// Created by aurailus on 14/04/19.
//

#ifndef ZEUS_BLOCKMODELENTITY_H
#define ZEUS_BLOCKMODELENTITY_H


#include "../../../game/entity/Entity.h"
#include "../../../def/block/graph/BlockModel.h"
#include "../../../def/GameDefs.h"

class BlockModelEntity : public Entity {
public:
    explicit BlockModelEntity(GameDefs &defs);

    void setModel(unsigned int blockID, unsigned short crackLevel);
    void clearModel();
private:
    void addFaces(unsigned int &indOffset, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<MeshPart> &meshParts);

    std::vector<std::shared_ptr<AtlasRef>> crackedFaces;

    unsigned int blockID;
    unsigned int crackLevel;

    GameDefs& defs;
};


#endif //ZEUS_BLOCKMODELENTITY_H
