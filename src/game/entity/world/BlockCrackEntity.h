//
// Created by aurailus on 14/04/19.
//

#pragma once

#include "../Entity.h"
#include "../../../def/LocalDefs.h"

class BlockCrackEntity : public Entity {
public:
    explicit BlockCrackEntity(LocalDefs &defs, glm::vec3 blockPos, unsigned int blockID);

    void update();
    void setNewDamage(float damage);

    glm::vec3 blockPos;
    unsigned int blockID;
    float targetDamage = 0;
    float damage = 0;
    double time = 0;
private:
    short crackLevel = -1;

    void addFaces(unsigned int &indOffset, std::vector<EntityVertex> &vertices, std::vector<unsigned int> &indices, std::vector<MeshPart> &meshParts);

    std::vector<std::shared_ptr<AtlasRef>> crackedFaces;
    LocalDefs& defs;
};
