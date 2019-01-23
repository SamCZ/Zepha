//
// Created by aurailus on 02/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#ifndef GLPROJECT_MESHPART_H
#define GLPROJECT_MESHPART_H

#define GLM_ENABLE_EXPERIMENTAL

#include <vector>
#include <vec4.hpp>
#include <gtx/normal.hpp>

#include "MeshMod.h"
#include "TextureAtlas.h"
#include "../../client/graphics/mesh/Vertex.h"

class MeshVertexIter;
class MeshIndexIter;

struct MeshPart {
public:
    MeshPart(std::vector<Vertex*>* vertices, std::vector<unsigned int>* indices, const char* texture, TextureAtlas* atlas);

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
    float modValue;

    std::vector<Vertex*>* vertices;
    std::vector<unsigned int>* indices;

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

#pragma clang diagnostic pop