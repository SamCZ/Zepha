//
// Created by aurailus on 25/11/18.
//

#ifndef GLPROJECT_MESH_H
#define GLPROJECT_MESH_H

#include <GL/glew.h>

class Mesh {
public:
    Mesh();

    void create(GLfloat *vertices, unsigned int *indices, unsigned int vertCount, unsigned int indCount);
    void draw();
    void cleanup();

    ~Mesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indCount;
};


#endif //GLPROJECT_MESH_H
