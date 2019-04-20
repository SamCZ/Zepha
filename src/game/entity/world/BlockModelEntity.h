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

    void setModel(BlockModel& model);
    void clearModel();
private:
    void addFaces(unsigned int &indOffset, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<MeshPart> &meshParts);

    float n = 0;

    GameDefs& defs;
};


#endif //ZEUS_BLOCKMODELENTITY_H
