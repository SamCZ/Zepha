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

void MeshGenerator::add_face(std::vector<float> *meshVerts, std::vector<unsigned int> *meshInds, int *indOffset,
              std::vector<float> *faceVerts, std::vector<unsigned int> *faceInds, int *x, int *y, int *z) {

    int numOfVertices = (int)(faceVerts->size() / 5); //Replace 5 with however many values there are per vert

    glm::vec3 normal;

    for (int i = 0; i < numOfVertices; i++) {
        //Vertices
        meshVerts->push_back((*faceVerts)[i*5 + 0] + *x);
        meshVerts->push_back((*faceVerts)[i*5 + 1] + *y);
        meshVerts->push_back((*faceVerts)[i*5 + 2] + *z);

        //TexCoords
        meshVerts->push_back((*faceVerts)[i*5 + 3]);
        meshVerts->push_back((*faceVerts)[i*5 + 4]);

        //Normals
        if (i % 4 == 0) {
            normal = glm::triangleNormal(
                    glm::vec3((*faceVerts)[(i  )*5 + 0], (*faceVerts)[(i  )*5 + 1], (*faceVerts)[(i  )*5 + 2]),
                    glm::vec3((*faceVerts)[(i+1)*5 + 0], (*faceVerts)[(i+1)*5 + 1], (*faceVerts)[(i+1)*5 + 2]),
                    glm::vec3((*faceVerts)[(i+2)*5 + 0], (*faceVerts)[(i+2)*5 + 1], (*faceVerts)[(i+2)*5 + 2]));
        }
        meshVerts->push_back(normal.x);
        meshVerts->push_back(normal.y);
        meshVerts->push_back(normal.z);
    }

    for (auto i : *faceInds) {
        meshInds->push_back(*indOffset + i);
    }

    *indOffset += numOfVertices;
}

void MeshGenerator::add_top_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z) {
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

    add_face(vertices, indices, lastIndex, &vec, &vec2, x, y, z);
}

void MeshGenerator::add_bottom_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z) {
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

    add_face(vertices, indices, lastIndex, &vec, &vec2, x, y, z);
}

void MeshGenerator::add_front_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z){
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

    add_face(vertices, indices, lastIndex, &vec, &vec2, x, y, z);
}

void MeshGenerator::add_back_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z){
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

    add_face(vertices, indices, lastIndex, &vec, &vec2, x, y, z);
}

void MeshGenerator::add_left_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z){
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

    add_face(vertices, indices, lastIndex, &vec, &vec2, x, y, z);
}

void MeshGenerator::add_right_face(std::vector<float> *vertices, std::vector<unsigned int> *indices, int *lastIndex, int *x, int *y, int *z){
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

    add_face(vertices, indices, lastIndex, &vec, &vec2, x, y, z);
}