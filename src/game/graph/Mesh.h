//
// Created by aurailus on 25/11/18.
//

#ifndef GLPROJECT_MESH_H
#define GLPROJECT_MESH_H

#include <GL/glew.h>
#include <vector>

class Mesh {
public:
    Mesh();

    void create(std::vector<float>* vertices, std::vector<unsigned int>* indices);
    void draw();
    void cleanup();

    ~Mesh();

private:
    GLuint VAO, VBO, IBO;
    GLsizei indCount;
};


#endif //GLPROJECT_MESH_H
