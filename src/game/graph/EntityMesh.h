//
// Created by aurailus on 18/06/19.
//

#ifndef ZEUS_ENTITYMESH_H
#define ZEUS_ENTITYMESH_H

#include <glew/glew.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

struct EntityVertex {
    glm::vec3 position;
    glm::vec4 colorData;
    float useTex;
    glm::vec3 normal;
};

#define STRIDE_OFFSET_ENTITY(m) sizeof(struct EntityVertex), (void *)offsetof(struct EntityVertex, m)

class EntityMesh {
public:
    EntityMesh();

    void create(const std::vector<EntityVertex>& vertices, const std::vector<unsigned int>& indices);
    void draw();
    void cleanup();

    ~EntityMesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indCount;
};


#endif //ZEUS_ENTITYMESH_H
