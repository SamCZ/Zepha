//
// Created by aurailus on 02/12/18.
//

#include <iostream>
#include "MeshPart.h"

MeshPart::MeshPart(Vertex* vertices, int vSize, unsigned int* indices, int iSize, const char* texture, MeshMod meshMod, float modValue) {
    construct(vertices, vSize, indices, iSize, texture, meshMod, modValue);
}

MeshPart::MeshPart(Vertex* vertices, int vSize, unsigned int* indices, int iSize, const char* texture) {
    construct(vertices, vSize, indices, iSize, texture, NONE, 0);
}

//Add normals and compute tex coordinates for the given Vertex array.

void MeshPart::construct(Vertex* vertices, int vSize, unsigned int *indices, int iSize, const char* texture, MeshMod meshMod, float modValue) {

    //Set the meshMod and modValue variables on the MeshPart

    this->meshMod = meshMod;
    this->modValue = modValue;

    //These vertex structs do (should) not have normals, so we will generate them here from the triangle information
    //To do this, we have to assume that each group of 3 indices is a triangle (which it would be hard for it to not be)
    //and that no vertexes are shared on corners or places where vectors should be interpolated.


    //Iterate through the indices to find all used vertices to add normals and adjust texture coordinates.
    Vertex *p1, *p2, *p3;
    glm::vec3 nml;

    for (int i = 0; i < iSize/3; i++) {
        //Get the three vertices
        p1 = &vertices[indices[i*3]];
        p2 = &vertices[indices[i*3 + 1]];
        p3 = &vertices[indices[i*3 + 2]];

        //Get the normal of the formed triangle
        nml = glm::triangleNormal(*(p1->pos), *(p2->pos), *(p3->pos));

        //Set the normal on the vertices
        p1->nml = new glm::vec3(nml.x, nml.y, nml.z);
        p2->nml = new glm::vec3(nml.x, nml.y, nml.z);
        p3->nml = new glm::vec3(nml.x, nml.y, nml.z);

        //TODO: use 'texture' varaible here
//        glm::vec4 texBase = glm::vec4(0, 0, 1, 1);
//        glm::vec2 texWidth = glm::vec2(texBase.x - texBase.z, texBase.y - texBase.w);

        //Adjust the texture coordinates to be relative to the texture requested.
    }

    //Assign the inputted values to the struct

    this->vertices = vertices;
    this->vSize = vSize;
    this->indices = indices;
    this->iSize = iSize;
}

int MeshPart::getVertexCount() {
    return vSize;
}

Vertex* MeshPart::getVertex(int index) {
    return &vertices[index];
}

int MeshPart::getIndexCount() {
    return iSize;
}

unsigned int MeshPart::getIndex(int index) {
    return indices[index];
}

MeshVertexIter* MeshPart::getVertexIterator() {
    return new MeshVertexIter(this);
}

MeshIndexIter* MeshPart::getIndexIterator() {
    return new MeshIndexIter(this);
}

void MeshPart::cleanup() {
    delete [] vertices;
    delete [] indices;
}

MeshPart::~MeshPart() {
    cleanup();
}

//Print information about this MeshPart into std::cout.
void MeshPart::debug() {
    std::cout << "Debugging MeshPart" << std::endl << "Vertices:" << std::endl;

    auto* vI = getVertexIterator();

    while (vI->hasNext()) {
        Vertex* v = vI->next();
        std::cout << v->pos->x << ", " << v->pos->y << ", " << v->pos->z << " | " << v->nml->x << ", " << v->nml->y << ", " << v->nml->z << std::endl;
    }

    auto* iI = getIndexIterator();
    bool first = true;

    std::cout << "Indices:" << std::endl;

    while (iI->hasNext()) {
        if (!first) {
            std::cout << ", ";
        }
        else first = false;
        std::cout << iI->next();
    }
    std::cout << std::endl;

    std::cout << "----------" << std::endl;
}

//Mesh Vertex Iterator
//Iterator to get the vertices of the MeshPart as a stream.

MeshVertexIter::MeshVertexIter(MeshPart* meshPart) {
    this->meshPart = meshPart;
    this->index = 0;
}

bool MeshVertexIter::hasNext() {
    return (index < this->meshPart->getVertexCount());
}

Vertex* MeshVertexIter::next() {
    if (hasNext()) return this->meshPart->getVertex(index++);
}

//Mesh Index Iterator
//Iterator to get the indices of the MeshPart as a stream

MeshIndexIter::MeshIndexIter(MeshPart *meshPart) {
    this->meshPart = meshPart;
    this->index = 0;
}

bool MeshIndexIter::hasNext() {
    return (index < this->meshPart->getIndexCount());
}

unsigned int MeshIndexIter::next() {
    if (hasNext()) return this->meshPart->getIndex(index++);
}