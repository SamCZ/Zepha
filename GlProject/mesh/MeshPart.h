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
#include "../engine/TextureAtlas.h"

class MeshVertexIter;
class MeshIndexIter;

struct MeshPart {
public:
    MeshPart(Vertex* vertices, int vSize, unsigned int* indices, int iSize, const char* texture, MeshMod meshMod, float modValue, TextureAtlas* atlas);
    MeshPart(Vertex* vertices, int vSize, unsigned int* indices, int iSize, const char* texture, TextureAtlas* atlas);

    int getVertexCount();
    Vertex* getVertex(int index);

    int getIndexCount();
    unsigned int getIndex(int index);

    MeshVertexIter* getVertexIterator();
    MeshIndexIter* getIndexIterator();

    void debug();

    void cleanup();
    ~MeshPart();
private:
    void construct(Vertex* vertices, int vSize, unsigned int* indices, int iSize, const char* texture, MeshMod meshMod, float modValue, TextureAtlas* atlas);

    float modValue;

    Vertex* vertices;
    int vSize;
    unsigned int* indices;
    int iSize;

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
