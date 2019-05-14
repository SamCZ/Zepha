//
// Created by aurailus on 14/04/19.
//

#ifndef ZEUS_BLOCKMODELENTITY_H
#define ZEUS_BLOCKMODELENTITY_H


#include "../../../game/entity/Entity.h"
#include "../../../def/block/graph/BlockModel.h"
#include "../../../def/GameDefs.h"

class BlockCrackEntity : public Entity {
public:
    explicit BlockCrackEntity(GameDefs &defs, glm::vec3 blockPos, unsigned int blockID);

    void update();
    void setNewDamage(float damage);

    glm::vec3 blockPos;
    unsigned int blockID;
    float targetDamage = 0;
    float damage = 0;
    double time = 0;
private:
    short crackLevel = -1;

    void addFaces(unsigned int &indOffset, std::vector<float> &vertices, std::vector<unsigned int> &indices, std::vector<MeshPart> &meshParts);

    std::vector<std::shared_ptr<AtlasRef>> crackedFaces;
    GameDefs& defs;
};


#endif //ZEUS_BLOCKMODELENTITY_H
