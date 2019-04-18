//
// Created by aurailus on 14/04/19.
//

#ifndef ZEUS_BLOCKMODELENTITY_H
#define ZEUS_BLOCKMODELENTITY_H


#include "../../engine/Entity.h"
#include "../../../generic/blocks/BlockModel.h"

class BlockModelEntity : public Entity {
public:
    explicit BlockModelEntity(DynamicAtlas *texAtlas);

    void setModel(BlockModel& model);
    void clearModel();
private:
    void addFaces(unsigned int &indOffset, vector<float> &vertices, vector<unsigned int> &indices, vector<MeshPart*> &meshParts);

    float n = 0;

    DynamicAtlas *texAtlas;
};


#endif //ZEUS_BLOCKMODELENTITY_H
