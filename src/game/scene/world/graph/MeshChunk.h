//
// Created by aurailus on 15/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "HidingNonVirtualFunction"

#ifndef GLPROJECT_MESHCHUNK_H
#define GLPROJECT_MESHCHUNK_H

#include "../../../entity/world/Entity.h"
#include "../../../../util/TransPos.h"

class MeshChunk : Drawable {
public:
    MeshChunk() = default;
    void build(std::vector<ChunkVertex>& vertices, std::vector<unsigned int>& indices);

    void draw(Renderer& renderer) override;

    void setPos(glm::vec3 pos);
    glm::vec3 getPos();

    ~MeshChunk();
private:
    void cleanup();
    glm::mat4 getModelMatrix();

    ChunkMesh* mesh = nullptr;
    glm::vec3 pos;
};


#endif //GLPROJECT_MESHCHUNK_H

#pragma clang diagnostic pop