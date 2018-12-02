//
// Created by aurailus on 01/12/18.
//

#ifndef GLPROJECT_MESHGENERATOR_H
#define GLPROJECT_MESHGENERATOR_H

#define GLM_ENABLE_EXPERIMENTAL

#include "MeshData.h"
#include <vector>
#include <gtx/normal.hpp>

class MeshGenerator {
public:
    MeshGenerator();

    MeshData* generate(int blocks[4][4][4]);
private:
    void add_face(std::vector<float> *meshVerts, std::vector<unsigned int> *meshInds, int *indOffset,
            std::vector<float> *faceVerts, std::vector<unsigned int> *faceInds, int *x, int *y, int *z);

    void add_top_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z);
    void add_bottom_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z);

    void add_front_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z);
    void add_back_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z);

    void add_left_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z);
    void add_right_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z);
};


#endif //GLPROJECT_MESHGENERATOR_H
