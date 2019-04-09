//
// Created by aurailus on 08/04/19.
//

#include "WireframeGenerator.h"

WireframeGenerator::WireframeGenerator(float xSize, float ySize, float zSize, float width, glm::vec3 color) {
    this->xSize = xSize;
    this->ySize = ySize;
    this->zSize = zSize;
    this->width = width;
    this->color = color;
}

Mesh *WireframeGenerator::build() {
    float hw = (width/2.0f);
    float w = width;
    float x = xSize;
    float y = ySize;
    float z = zSize;

    createBox( -hw,  -hw,  -hw,   w, y+w,   w);
    createBox(x-hw,  -hw,  -hw,   w, y+w,   w);
    createBox(x-hw,  -hw, z-hw,   w, y+w,   w);
    createBox( -hw,  -hw, z-hw,   w, y+w,   w);

    createBox( -hw,  -hw,  -hw, x+w,   w,   w);
    createBox( -hw, y-hw,  -hw, x+w,   w,   w);
    createBox( -hw, y-hw, z-hw, x+w,   w,   w);
    createBox( -hw,  -hw, z-hw, x+w,   w,   w);

    createBox( -hw,  -hw,  -hw,   w,   w, z+w);
    createBox( -hw, y-hw,  -hw,   w,   w, z+w);
    createBox(x-hw, y-hw,  -hw,   w,   w, z+w);
    createBox(x-hw,  -hw,  -hw,   w,   w, z+w);

    auto m = new Mesh();
    m->create(&vertices, &indices);
    return m;
}

void WireframeGenerator::createBox(float x, float y, float z, float xSize, float ySize, float zSize) {
    auto c = color;

    std::vector<float> myVerts {
            /*0*/ x, y, z, 0, c.x, c.y, c.z, 1, 0, 0, 0,
            /*1*/ x + xSize, y, z, 0, c.x, c.y, c.z, 1, 0, 0, 0,
            /*2*/ x + xSize, y, z + zSize, 0, c.x, c.y, c.z, 1, 0, 0, 0,
            /*3*/ x, y, z + zSize, 0, c.x, c.y, c.z, 1, 0, 0, 0,

            /*4*/ x, y + ySize, z, 0, c.x, c.y, c.z, 1, 0, 0, 0,
            /*5*/ x + xSize, y + ySize, z, 0, c.x, c.y, c.z, 1, 0, 0, 0,
            /*6*/ x + xSize, y + ySize, z + zSize, 0, c.x, c.y, c.z, 1, 0, 0, 0,
            /*7*/ x, y + ySize, z + zSize, 0, c.x, c.y, c.z, 1, 0, 0, 0,
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
