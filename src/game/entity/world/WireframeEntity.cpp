//
// Created by aurailus on 08/04/19.
//

#include "WireframeEntity.h"


WireframeEntity::WireframeEntity(const std::vector<SelectionBox>& boxes, float width, glm::vec3 color) {
    this->width = width;
    this->color = color;

    buildMesh(boxes);
}

void WireframeEntity::updateMesh(const std::vector<SelectionBox>& boxes, float width) {
    this->width = width;

    buildMesh(boxes);
}


void WireframeEntity::buildMesh(const std::vector<SelectionBox>& boxes) {
    indOffset = 0;

    vertices.clear();
    indices.clear();

    for (auto& box : boxes) {
        glm::vec3 a = box.a;
        glm::vec3 b = box.b - box.a;

        createBox(a, b, 0,   0, 0,   0, b.y, 0);
        createBox(a, b, b.x, 0, 0,   0, b.y, 0);
        createBox(a, b, b.x, 0, b.z, 0, b.y, 0);
        createBox(a, b, 0,   0, b.z, 0, b.y, 0);

        createBox(a, b, 0, 0,   0,   b.x, 0, 0);
        createBox(a, b, 0, b.y, 0,   b.x, 0, 0);
        createBox(a, b, 0, b.y, b.z, b.x, 0, 0);
        createBox(a, b, 0, 0,   b.z, b.x, 0, 0);

        createBox(a, b, 0,   0,   0, 0, 0, b.z);
        createBox(a, b, 0,   b.y, 0, 0, 0, b.z);
        createBox(a, b, b.x, b.y, 0, 0, 0, b.z);
        createBox(a, b, b.x, 0,   0, 0, 0, b.z);
    }

    auto m = new EntityMesh();
    m->create(vertices, indices);
    setMesh(m);
}

void WireframeEntity::createBox(glm::vec3 a, glm::vec3 b, float x, float y, float z, float xSize, float ySize, float zSize) {
    float hw = (width/2.0f);
    float w = width;
    auto  c = color;

    std::vector<EntityVertex> myVerts {
    /*0*/ {{x - hw + a.x,             y - hw + a.y,             z - hw + a.z            }, {c, 1}, {1, 1, 1}, false, {0, 1, 0}},
    /*1*/ {{x - hw + a.x + xSize + w, y - hw + a.y,             z - hw + a.z            }, {c, 1}, {1, 1, 1}, false, {0, 1, 0}},
    /*2*/ {{x - hw + a.x + xSize + w, y - hw + a.y,             z - hw + a.z + zSize + w}, {c, 1}, {1, 1, 1}, false, {0, 1, 0}},
    /*3*/ {{x - hw + a.x,             y - hw + a.y,             z - hw + a.z + zSize + w}, {c, 1}, {1, 1, 1}, false, {0, 1, 0}},

    /*4*/ {{x - hw + a.x,             y - hw + a.y + ySize + w, z - hw + a.z            }, {c, 1}, {1, 1, 1}, false, {0, 1, 0}},
    /*5*/ {{x - hw + a.x + xSize + w, y - hw + a.y + ySize + w, z - hw + a.z            }, {c, 1}, {1, 1, 1}, false, {0, 1, 0}},
    /*6*/ {{x - hw + a.x + xSize + w, y - hw + a.y + ySize + w, z - hw + a.z + zSize + w}, {c, 1}, {1, 1, 1}, false, {0, 1, 0}},
    /*7*/ {{x - hw + a.x,             y - hw + a.y + ySize + w, z - hw + a.z + zSize + w}, {c, 1}, {1, 1, 1}, false, {0, 1, 0}},
    };

    std::vector<unsigned int> myInds {
        0, 1, 2, 2, 3, 0,
        4, 7, 6, 6, 5, 4,
        0, 4, 5, 5, 1, 0,
        3, 2, 6, 6, 7, 3,
        0, 3, 7, 7, 4, 0,
        1, 5, 6, 6, 2, 1,
    };

    vertices.insert(vertices.end(), myVerts.begin(), myVerts.end());
    for (auto i : myInds) indices.push_back(i + indOffset);

    indOffset += 8;
}