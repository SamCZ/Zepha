//
// Created by aurailus on 14/04/19.
//

#pragma once

#include "../Entity.h"

class BlockDef;
class AtlasRef;
class MeshPart;
class EntityVertex;
class TextureAtlas;

class BlockCrackEntity : public Entity {
public:
    BlockCrackEntity(BlockDef &def, TextureAtlas& textureAtlas, glm::ivec3 pos);

    void update();
    void addDamage(double damage);
    void setDamage(double damage);

    int maxHealth = 0;
    double damage = 0;
    double damagePending = 0;

    double time = 0;
    BlockDef& def;
private:
    void addFaces(unsigned int &indOffset, std::vector<EntityVertex> &vertices, std::vector<unsigned int> &indices, std::vector<MeshPart> &meshParts);

    short crackLevel = -1;

    TextureAtlas& textureAtlas;
    std::vector<std::shared_ptr<AtlasRef>> crackedFaces;
};
