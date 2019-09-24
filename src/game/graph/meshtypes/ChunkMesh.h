//
// Created by aurailus on 25/11/18.
//

#pragma once

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
