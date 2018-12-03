//
// Created by aurailus on 01/12/18.
//

#include <cstdio>
#include "MeshGenerator.h"
#include "../engine/Timer.h"

MeshGenerator::MeshGenerator() {
    vertices = new std::vector<float>;
    indices = new std::vector<unsigned int>;
    indOffset = 0;
}

MeshData* MeshGenerator::build(int blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]) {
    auto t = Timer("MeshGen");

    vertices->reserve(32768);
    indices->reserve(8192);

    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            for (int k = 0; k < CHUNK_SIZE; k++) {
                if (blocks[i][j][k] == 1) {
                    add_top_face(&k, &j, &i);
                    add_bottom_face(&k, &j, &i);

                    add_front_face(&k, &j, &i);
                    add_back_face(&k, &j, &i);

                    add_left_face(&k, &j, &i);
                    add_right_face(&k, &j, &i);
                }
            }
        }
    }

    vertices->shrink_to_fit();
    indices->shrink_to_fit();

    auto* m = new MeshData();
    m->vertices = *vertices;
    m->indices = *indices;

    t.elapsed();

    return m;
}

void MeshGenerator::add_vertices(std::vector<float> *verts, std::vector<unsigned int> *inds, int *x, int *y, int *z) {
    //How many values there are per vertex
    const unsigned int VERT_SIZE = 5;

    //Total vertices in the array
    int numOfVertices = (int)(verts->size() / VERT_SIZE);

    glm::vec3 normal;

    for (int i = 0; i < numOfVertices; i++) {
        //Vertices
        vertices->push_back((*verts)[i*VERT_SIZE + 0] + *x);
        vertices->push_back((*verts)[i*VERT_SIZE + 1] + *y);
        vertices->push_back((*verts)[i*VERT_SIZE + 2] + *z);

        //TexCoords
        vertices->push_back((*verts)[i*VERT_SIZE + 3]);
        vertices->push_back((*verts)[i*VERT_SIZE + 4]);

        //Normals
        if (i % 4 == 0) {
            normal = glm::triangleNormal(
                    glm::vec3((*verts)[(i  )*VERT_SIZE + 0], (*verts)[(i  )*VERT_SIZE + 1], (*verts)[(i  )*VERT_SIZE + 2]),
                    glm::vec3((*verts)[(i+1)*VERT_SIZE + 0], (*verts)[(i+1)*VERT_SIZE + 1], (*verts)[(i+1)*VERT_SIZE + 2]),
                    glm::vec3((*verts)[(i+2)*VERT_SIZE + 0], (*verts)[(i+2)*VERT_SIZE + 1], (*verts)[(i+2)*VERT_SIZE + 2]));
        }
        vertices->push_back(normal.x);
        vertices->push_back(normal.y);
        vertices->push_back(normal.z);
    }

    for (auto i : *inds) {
        indices->push_back(indOffset + i);
    }

    indOffset += numOfVertices;
}

void MeshGenerator::add_top_face(int *x, int *y, int *z) {
    float face_vertices[] {
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            0.5f,  0.5f, -0.5f, 0.0f, 1.0f
    };
    unsigned int face_indices[] {
            0, 1, 2, 2, 3, 0
    };

    std::vector<float> vec(std::begin(face_vertices), std::end(face_vertices));
    std::vector<unsigned int> vec2(std::begin(face_indices), std::end(face_indices));

    add_vertices(&vec, &vec2, x, y, z);
}

void MeshGenerator::add_bottom_face(int *x, int *y, int *z) {
    float face_vertices[] {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,
    };
    unsigned int face_indices[] {
            0, 1, 2, 2, 3, 0
    };

    std::vector<float> vec(std::begin(face_vertices), std::end(face_vertices));
    std::vector<unsigned int> vec2(std::begin(face_indices), std::end(face_indices));

    add_vertices(&vec, &vec2, x, y, z);
}

void MeshGenerator::add_front_face(int *x, int *y, int *z){
    float face_vertices[] {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
    };
    unsigned int face_indices[] {
            0, 1, 2, 2, 3, 0
    };

    std::vector<float> vec(std::begin(face_vertices), std::end(face_vertices));
    std::vector<unsigned int> vec2(std::begin(face_indices), std::end(face_indices));

    add_vertices(&vec, &vec2, x, y, z);
}

void MeshGenerator::add_back_face(int *x, int *y, int *z){
    float face_vertices[] {
            0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
    };
    unsigned int face_indices[] {
            0, 1, 2, 2, 3, 0
    };

    std::vector<float> vec(std::begin(face_vertices), std::end(face_vertices));
    std::vector<unsigned int> vec2(std::begin(face_indices), std::end(face_indices));

    add_vertices(&vec, &vec2, x, y, z);
}

void MeshGenerator::add_left_face(int *x, int *y, int *z){
    float face_vertices[] {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
    };
    unsigned int face_indices[] {
            0, 1, 2, 2, 3, 0
    };

    std::vector<float> vec(std::begin(face_vertices), std::end(face_vertices));
    std::vector<unsigned int> vec2(std::begin(face_indices), std::end(face_indices));

    add_vertices(&vec, &vec2, x, y, z);
}

void MeshGenerator::add_right_face(int *x, int *y, int *z){
    float face_vertices[] {
            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
            0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
    };
    unsigned int face_indices[] {
            0, 1, 2, 2, 3, 0
    };

    std::vector<float> vec(std::begin(face_vertices), std::end(face_vertices));
    std::vector<unsigned int> vec2(std::begin(face_indices), std::end(face_indices));

    add_vertices(&vec, &vec2, x, y, z);
}

void MeshGenerator::cleanup() {

}

MeshGenerator::~MeshGenerator() {
    cleanup();
}