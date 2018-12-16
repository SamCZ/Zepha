//
// Created by aurailus on 14/12/18.
//

#ifndef GLPROJECT_WORLD_H
#define GLPROJECT_WORLD_H

#include <map>
#include <vec3.hpp>
#include <unordered_set>
#include <iostream>
#include <gtc/type_ptr.hpp>

#include "BlockChunk.h"
#include "MeshChunk.h"
#include "../blocks/BlockAtlas.h"

class World {
public:
    World() = default;
    explicit World(BlockAtlas* atlas);

    void newChunk(glm::vec3* pos, BlockChunk* c);

    void update();
    void draw(GLint modelUni);

    ~World() = default;
private:
    //Note to self:
    //The same glm::vec3 pointer is used for the blockChunk and meshChunk maps. If discarding a block/meshChunk, keep
    //this in mind. If this needs to be changed later change the 'update' method.
    std::map<glm::vec3*, BlockChunk*> blockChunks;
    std::map<glm::vec3*, MeshChunk*> meshChunks;

    std::unordered_set<glm::vec3*> meshGenQueue;

    BlockAtlas* blockAtlas;
};


#endif //GLPROJECT_WORLD_H
