//
// Created by aurailus on 08/04/19.
//

#include "WireframeGenerator.h"


WireframeGenerator::WireframeGenerator(glm::vec3 a, glm::vec3 b, float width, glm::vec3 color) {

    this->a = a;
    this->b = b - a;
    this->width = width;
    this->color = color;
}

Mesh *WireframeGenerator::build() {
    float hw = (width/2.0f);
    float w = width;

    createBox(   -hw,    -hw,    -hw,     w, b.y+w,     w);
    createBox(b.x-hw,    -hw,    -hw,     w, b.y+w,     w);
    createBox(b.x-hw,    -hw, b.z-hw,     w, b.y+w,     w);
    createBox(   -hw,    -hw, b.z-hw,     w, b.y+w,     w);

    createBox(   -hw,    -hw,    -hw, b.x+w,     w,     w);
    createBox(   -hw, b.y-hw,    -hw, b.x+w,     w,     w);
    createBox(   -hw, b.y-hw, b.z-hw, b.x+w,     w,     w);
    createBox(   -hw,    -hw, b.z-hw, b.x+w,     w,     w);

    createBox(   -hw,    -hw,    -hw,     w,     w, b.z+w);
    createBox(   -hw, b.y-hw,    -hw,     w,     w, b.z+w);
    createBox(b.x-hw, b.y-hw,    -hw,     w,     w, b.z+w);
    createBox(b.x-hw,    -hw,    -hw,     w,     w, b.z+w);

    auto m = new Mesh();
    m->create(&vertices, &indices);
    return m;
}

void WireframeGenerator::createBox(float x, float y, float z, float xSize, float ySize, float zSize) {
    auto c = color;

    std::vector<float> myVerts {
        /*0*/ x + a.x,         y + a.y,         z + a.z,         0, c.x, c.y, c.z, 0.5, 0, 0, 0,
        /*1*/ x + a.x + xSize, y + a.y,         z + a.z,         0, c.x, c.y, c.z, 0.5, 0, 0, 0,
        /*2*/ x + a.x + xSize, y + a.y,         z + a.z + zSize, 0, c.x, c.y, c.z, 0.5, 0, 0, 0,
        /*3*/ x + a.x,         y + a.y,         z + a.z + zSize, 0, c.x, c.y, c.z, 0.5, 0, 0, 0,

        /*4*/ x + a.x,         y + a.y + ySize, z + a.z,         0, c.x, c.y, c.z, 0.5, 0, 0, 0,
        /*5*/ x + a.x + xSize, y + a.y + ySize, z + a.z,         0, c.x, c.y, c.z, 0.5, 0, 0, 0,
        /*6*/ x + a.x + xSize, y + a.y + ySize, z + a.z + zSize, 0, c.x, c.y, c.z, 0.5, 0, 0, 0,
        /*7*/ x + a.x,         y + a.y + ySize, z + a.z + zSize, 0, c.x, c.y, c.z, 0.5, 0, 0, 0,
    };

    std::vector<unsigned int> myInds {
        0, 1, 2, 2, 3, 0,
        4, 7, 6, 6, 5, 4,
        0, 4, 5, 5, 1, 0,
        3, 2, 6, 6, 7, 3,
        0, 3, 7, 7, 4, 0,
        1, 5, 6, 6, 2, 1,
    };

    for (auto f : myVerts) {
        vertices.push_back(f);
    }

    for (auto i : myInds) {
        indices.push_back(i + indOffset);
    }

    indOffset += 8;
}