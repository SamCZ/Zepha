//
// Created by aurailus on 01/12/18.
//

#include <cstdio>
#include "MeshGenerator.h"

MeshGenerator::MeshGenerator() = default;

MeshData* MeshGenerator::generate(int blocks[4][4][4]) {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    int lastIndex = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            for (int k = 0; k < 4; k++) {
                if (blocks[i][j][k] == 1) {
                    add_top_face(&vertices, &indices, &lastIndex, &k, &j, &i);
                    add_bottom_face(&vertices, &indices, &lastIndex, &k, &j, &i);

                    add_front_face(&vertices, &indices, &lastIndex, &k, &j, &i);
                    add_back_face(&vertices, &indices, &lastIndex, &k, &j, &i);

                    add_left_face(&vertices, &indices, &lastIndex, &k, &j, &i);
                    add_right_face(&vertices, &indices, &lastIndex, &k, &j, &i);
                }
            }
        }
    }

    auto* m = new MeshData();
    m->vertices = vertices;
    m->indices = indices;

    return m;
}

void MeshGenerator::add_top_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z) {
    float top_vertices[] {
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };
    unsigned int top_indices[] {
            0, 1, 2, 2, 3, 0
    };
    int indexCount = 4;

    for (int i = 0; i < (sizeof(top_vertices) / sizeof(top_vertices[0]) / 5); i++) {
        vertices->push_back(top_vertices[i*5 + 0] + *x);
        vertices->push_back(top_vertices[i*5 + 1] + *y);
        vertices->push_back(top_vertices[i*5 + 2] + *z);
        vertices->push_back(top_vertices[i*5 + 3]);
        vertices->push_back(top_vertices[i*5 + 4]);
    }

    for (auto i : top_indices) {
        indices->push_back(*lastIndex + i);
    }

    *lastIndex += indexCount;
}

void MeshGenerator::add_bottom_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z) {
    float top_vertices[] {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
    };
    unsigned int top_indices[] {
            0, 1, 2, 2, 3, 0
    };
    int indexCount = 4;

    for (int i = 0; i < (sizeof(top_vertices) / sizeof(top_vertices[0]) / 5); i++) {
        vertices->push_back(top_vertices[i*5 + 0] + *x);
        vertices->push_back(top_vertices[i*5 + 1] + *y);
        vertices->push_back(top_vertices[i*5 + 2] + *z);
        vertices->push_back(top_vertices[i*5 + 3]);
        vertices->push_back(top_vertices[i*5 + 4]);
    }

    for (auto i : top_indices) {
        indices->push_back(*lastIndex + i);
    }

    *lastIndex += indexCount;
}

void MeshGenerator::add_front_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z){
    float front_vertices[] {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    };
    unsigned int front_indices[] {
            0, 1, 2, 2, 3, 0
    };
    int indexCount = 4;

    for (int i = 0; i < (sizeof(front_vertices) / sizeof(front_vertices[0]) / 5); i++) {
        vertices->push_back(front_vertices[i*5 + 0] + *x);
        vertices->push_back(front_vertices[i*5 + 1] + *y);
        vertices->push_back(front_vertices[i*5 + 2] + *z);
        vertices->push_back(front_vertices[i*5 + 3]);
        vertices->push_back(front_vertices[i*5 + 4]);
    }

    for (auto i : front_indices) {
        indices->push_back(*lastIndex + i);
    }

    *lastIndex += indexCount;
}

void MeshGenerator::add_back_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z){
    float front_vertices[] {
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    };
    unsigned int front_indices[] {
            0, 1, 2, 2, 3, 0
    };
    int indexCount = 4;

    for (int i = 0; i < (sizeof(front_vertices) / sizeof(front_vertices[0]) / 5); i++) {
        vertices->push_back(front_vertices[i*5 + 0] + *x);
        vertices->push_back(front_vertices[i*5 + 1] + *y);
        vertices->push_back(front_vertices[i*5 + 2] + *z);
        vertices->push_back(front_vertices[i*5 + 3]);
        vertices->push_back(front_vertices[i*5 + 4]);
    }

    for (auto i : front_indices) {
        indices->push_back(*lastIndex + i);
    }

    *lastIndex += indexCount;
}

void MeshGenerator::add_left_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z){
    float front_vertices[] {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    };
    unsigned int front_indices[] {
            0, 1, 2, 2, 3, 0
    };
    int indexCount = 4;

    for (int i = 0; i < (sizeof(front_vertices) / sizeof(front_vertices[0]) / 5); i++) {
        vertices->push_back(front_vertices[i*5 + 0] + *x);
        vertices->push_back(front_vertices[i*5 + 1] + *y);
        vertices->push_back(front_vertices[i*5 + 2] + *z);
        vertices->push_back(front_vertices[i*5 + 3]);
        vertices->push_back(front_vertices[i*5 + 4]);
    }

    for (auto i : front_indices) {
        indices->push_back(*lastIndex + i);
    }

    *lastIndex += indexCount;
}

void MeshGenerator::add_right_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z){
    float front_vertices[] {
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    };
    unsigned int front_indices[] {
            0, 1, 2, 2, 3, 0
    };
    int indexCount = 4;

    for (int i = 0; i < (sizeof(front_vertices) / sizeof(front_vertices[0]) / 5); i++) {
        vertices->push_back(front_vertices[i*5 + 0] + *x);
        vertices->push_back(front_vertices[i*5 + 1] + *y);
        vertices->push_back(front_vertices[i*5 + 2] + *z);
        vertices->push_back(front_vertices[i*5 + 3]);
        vertices->push_back(front_vertices[i*5 + 4]);
    }

    for (auto i : front_indices) {
        indices->push_back(*lastIndex + i);
    }

    *lastIndex += indexCount;
}