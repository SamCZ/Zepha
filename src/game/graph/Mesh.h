//
// Created by aurailus on 24/08/19.
//

#ifndef ZEUS_MESH_H
#define ZEUS_MESH_H

#include <glew/glew.h>

class Mesh {
public:
    Mesh() = default;

    void cleanup();
    virtual void draw();

    ~Mesh();
protected:
    void genArrays(GLuint vboLength, GLuint iboLength, const void* verticesPtr, const void* indicesPtr);
    void createVertexAttrib(GLuint offset, GLuint size, GLenum type, GLsizei stride, const void* pointer);

    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint IBO = 0;
    GLsizei indCount = 0;
};

#endif //ZEUS_MESH_H
