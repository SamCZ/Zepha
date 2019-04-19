//
// Created by aurailus on 14/04/19.
//

#ifndef ZEUS_BLOCKMODELENTITY_H
#define ZEUS_BLOCKMODELENTITY_H


#include "../../../game/entity/Entity.h"
#include "../../../def/block/graph/BlockModel.h"

class BlockModelEntity : public Entity {
public:
    explicit BlockModelEntity(TextureAtlas *texAtlas);

    void setModel(BlockModel& model);
    void clearModel();
private:
    void addFaces(unsigned int &indOffset, vector<float> &vertices, vector<unsigned int> &indices, vector<MeshPart*> &meshParts);

    float n = 0;

    TextureAtlas *texAtlas;
};


#endif //ZEUS_BLOCKMODELENTITY_H
