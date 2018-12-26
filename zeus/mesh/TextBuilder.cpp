//
// Created by aurailus on 25/12/18.
//

#include "TextBuilder.h"

Mesh* TextBuilder::build(std::string text) {
    auto textVertices = new std::vector<float>();
    auto textIndices = new std::vector<unsigned int>();

    float texPosWidth  = 1/128.0f * (float)w;
    float texPosHeight = 1/54.0f  * (float)h;

    for (unsigned int i = 0; i < text.length(); i++) {
        char letter = text[i];

        int baseIndex = (int)letter - 32;

        float left = i; //Cast to float so the vector doesn't error

        float texPosX = baseIndex%18 / 18.0f; //18 = Characters in row
        float texPosY = baseIndex/18 / 6.0f;  //6 = Number of columns

        auto letterVerts = std::vector<float> {
                left * w,     0, 0,  texPosX,               texPosY,                 0, 0, 0,
                left * w + w, 0, 0,  texPosX + texPosWidth, texPosY,                 0, 0, 0,
                left * w + w, h, 0,  texPosX + texPosWidth, texPosY + texPosHeight,  0, 0, 0,
                left * w,     h, 0,  texPosX,               texPosY + texPosHeight,  0, 0, 0,
        };

        for (float f : letterVerts) textVertices->push_back(f);

        textIndices->push_back(i*4);
        textIndices->push_back(3 + i*4);
        textIndices->push_back(1 + i*4);
        textIndices->push_back(3 + i*4);
        textIndices->push_back(2 + i*4);
        textIndices->push_back(1 + i*4);
    }

    Mesh* m = new Mesh();
    m->create(textVertices, textIndices);

    delete textVertices;
    delete textIndices;

    return m;
}

