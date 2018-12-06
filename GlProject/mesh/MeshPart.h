//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_MESHPART_H
#define GLPROJECT_MESHPART_H

#define GLM_ENABLE_EXPERIMENTAL

#include <vec4.hpp>
#include <gtx/normal.hpp>
#include "MeshMod.h"
#include "Vertex.h"

class MeshVertexIter;
class MeshIndexIter;

struct MeshPart {
public:
    MeshPart(Vertex* vertices, int vSize, unsigned int* indices, int iSize, const char* texture, MeshMod meshMod, float modValue);
    MeshPart(Vertex* vertices, int vSize, unsigned int* indices, int iSize, const char* texture);

    int getVertexCount();
    Vertex* getVertex(int index);

    int getIndexCount();
    unsigned int getIndex(int index);

    MeshVertexIter* getVertexIterator();
    MeshIndexIter* getIndexIterator();

    void debug();
private:
    void construct(Vertex* vertices, int vSize, unsigned int* indices, int iSize, const char* texture, MeshMod meshMod, float modValue);

    //TODO: Free these values from memory
    float modValue;
    Vertex* vertices;
    int vSize;
    unsigned int* indices;

    int iSize;

    const char* texture;
    MeshMod meshMod;
};

class MeshVertexIter {
public:
    explicit MeshVertexIter(MeshPart* meshPart);

    bool hasNext();
    Vertex* next();
private:
    int index;
    MeshPart* meshPart;
};

class MeshIndexIter {
public:
    explicit MeshIndexIter(MeshPart* meshPart);

    bool hasNext();
    unsigned int next();
private:
    int index;
    MeshPart* meshPart;
};


#endif //GLPROJECT_MESHPART_H
