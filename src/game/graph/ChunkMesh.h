//
// Created by aurailus on 25/11/18.
//

#ifndef GLPROJECT_MESH_H
#define GLPROJECT_MESH_H

#include <vector>
#include "Mesh.h"
#include "ChunkVertex.h"

class ChunkMesh : public Mesh {
public:
    ChunkMesh() = default;
    ChunkMesh(const ChunkMesh& o);
    void create(const std::vector<ChunkVertex>& vertices, const std::vector<unsigned int>& indices);
    ~ChunkMesh() = default;
};

#endif //GLPROJECT_MESH_H
