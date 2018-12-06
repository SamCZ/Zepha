//
// Created by aurailus on 01/12/18.
//

#ifndef GLPROJECT_MESHGENERATOR_H
#define GLPROJECT_MESHGENERATOR_H

#define GLM_ENABLE_EXPERIMENTAL

#include "BlockModel.h"
#include <vector>
#include <gtx/normal.hpp>

const int CHUNK_SIZE = 16;

struct MeshData;

class MeshGenerator {
public:
    MeshGenerator();
    MeshData* build(int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE], BlockModel* model);
    ~MeshGenerator();
private:
    std::vector<float>* vertices;
    std::vector<unsigned int>* indices;
    unsigned int indOffset;

    //TODO:: Replace the pointers to ints to non-pointer shorts(?)
    void addBlockModel(int* x, int* y, int* z, BlockModel* model);

    void add_vertices(std::vector<float> *verts, std::vector<unsigned int> *inds, int *x, int *y, int *z);

    void add_top_face(int *x, int *y, int *z);
    void add_bottom_face(int *x, int *y, int *z);

    void add_front_face(int *x, int *y, int *z);
    void add_back_face(int *x, int *y, int *z);

    void add_left_face(int *x, int *y, int *z);
    void add_right_face(int *x, int *y, int *z);

    void cleanup();
};

//Temporary class for storing Vertices and Indices.
//TODO: Replace this class with pointers to new Vertices and Indices in the build method.
struct MeshData {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif //GLPROJECT_MESHGENERATOR_H
