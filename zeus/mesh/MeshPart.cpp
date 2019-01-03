//
// Created by aurailus on 02/12/18.
//

#include "MeshPart.h"

MeshPart::MeshPart(std::vector<Vertex*>* vertices, std::vector<unsigned int>* indices, const char* texture, TextureAtlas* atlas) {

    this->vertices = vertices;
    this->indices = indices;

    this->meshMod = NONE;
    this->modValue = 0;

    //These vertex structs do (should) not have normals, so we will generate them here from the triangle information
    //To do this, we have to assume that each group of 3 indices is a triangle (which it would be hard for it to not be)
    //and that no vertexes are shared on corners or places where vectors should be interpolated.

    //Iterate through the indices to find all used vertices to add normals and adjust texture coordinates.
    Vertex *p1, *p2, *p3;
    glm::vec3 normal;

    for (int i = 0; i < indices->size()/3; i++) {
        //Get the three vertices
        p1 = (*vertices)[(*indices)[i*3]];
        p2 = (*vertices)[(*indices)[i*3 + 1]];
        p3 = (*vertices)[(*indices)[i*3 + 2]];

//        std::cout << p1->pos->x << ", " << p1->pos->y << ", " << p1->pos->z << std::endl;

        //Get the normal of the formed triangle
        normal = glm::triangleNormal(*(p1->pos), *(p2->pos), *(p3->pos));
        auto* nml = new glm::vec3(normal.x, normal.y, normal.z);

        //Set the normal on the vertices
        p1->nml = nml;
        p2->nml = nml;
        p3->nml = nml;
    }

    auto texString = std::string(texture);
    auto uv = atlas->getUVs(&texString);

//	std::cout << uv->x << ", " << uv->y << ", " << uv->z << ", " << uv->w << ", " << std::endl;

    //Iterate through the vertices to adjust the texture coordinates to fit the textureAtlas.
    for (int i = 0; i < vertices->size(); i++) {
        Vertex* vertex = (*vertices)[i];

        vertex->tex->x = uv->x + ((uv->z - uv->x) * vertex->tex->x);
        vertex->tex->y = uv->y + ((uv->w - uv->y) * vertex->tex->y);
    }
}

int MeshPart::getVertexCount() {
    return (int)vertices->size();
}

Vertex* MeshPart::getVertex(int index) {
    return (*vertices)[index];
}

int MeshPart::getIndexCount() {
    return (int)indices->size();
}

unsigned int MeshPart::getIndex(int index) {
    return (*indices)[index];
}

MeshVertexIter* MeshPart::getVertexIterator() {
    return new MeshVertexIter(this);
}

MeshIndexIter* MeshPart::getIndexIterator() {
    return new MeshIndexIter(this);
}

void MeshPart::cleanup() {
    delete vertices;
    delete indices;
    //TODO: Clean the values inside of the vectors
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