//
// Created by aurailus on 25/11/18.
//

#include "ChunkMesh.h"

ChunkMesh::ChunkMesh(const ChunkMesh &o) {
    throw "Copy constructor for ChunkMesh is not supported! Throwing.";
}

void ChunkMesh::create(const std::vector<ChunkVertex>& vertices, const std::vector<unsigned int>& indices) {
    indCount = static_cast<GLsizei>(indices.size());

    genArrays(static_cast<unsigned int>(vertices.size() * sizeof(ChunkVertex)),
              static_cast<unsigned int>(indices.size() * sizeof(unsigned int)),
              &vertices.front(), &indices.front());

    unsigned int idx = 0;
    createVertexAttrib(idx++, 3, GL_FLOAT, STRIDE_OFFSET_CHUNK(position));
    createVertexAttrib(idx++, 2, GL_FLOAT, STRIDE_OFFSET_CHUNK(texCoords));
    createVertexAttrib(idx++, 1, GL_FLOAT, STRIDE_OFFSET_CHUNK(normal));
    createVertexAttrib(idx++, 1, GL_FLOAT, STRIDE_OFFSET_CHUNK(shaderMod));
    createVertexAttrib(idx  , 3, GL_FLOAT, STRIDE_OFFSET_CHUNK(modValues));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}