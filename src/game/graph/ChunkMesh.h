//
// Created by aurailus on 25/11/18.
//

#ifndef GLPROJECT_MESH_H
#define GLPROJECT_MESH_H

#include <GL/glew.h>
#include <vec3.hpp>
#include <vec4.hpp>
#include <vector>
#include <vec2.hpp>

struct ChunkVertex {
    glm::vec3 position;
    glm::vec2 texCoords;
    float     normal;
    float     shaderMod;
    glm::vec3 modValues;
};

#define STRIDE_OFFSET_CHUNK(m) sizeof(struct ChunkVertex), (void *)offsetof(struct ChunkVertex, m)

class ChunkMesh {
public:
    ChunkMesh();

    void create(const std::vector<ChunkVertex>& vertices, const std::vector<unsigned int>& indices);
    void draw();
    void cleanup();

    ~ChunkMesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indCount;
};


#endif //GLPROJECT_MESH_H
