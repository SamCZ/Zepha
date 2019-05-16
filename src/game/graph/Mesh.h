//
// Created by aurailus on 25/11/18.
//

#ifndef GLPROJECT_MESH_H
#define GLPROJECT_MESH_H

#include <GL/glew.h>
#include <vec3.hpp>
#include <vec4.hpp>
#include <vector>

struct Vertex {
    glm::vec3 position;

    float useTex;
    glm::vec4 texCoords;

    glm::vec3 normal;
};

#define STRIDE_OFFSET(m) sizeof(struct Vertex), (void *)offsetof(struct Vertex, m)

class Mesh {
public:
    Mesh();

    void create(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
    void draw();
    void cleanup();

    ~Mesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indCount;
};


#endif //GLPROJECT_MESH_H
