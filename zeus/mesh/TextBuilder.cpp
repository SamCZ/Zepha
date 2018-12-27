//
// Created by aurailus on 25/12/18.
//

#include "TextBuilder.h"

Mesh* TextBuilder::build(std::string text) {
    auto textVertices = new std::vector<float>();
    auto textIndices = new std::vector<unsigned int>();

    float texPosWidth  = 1/128.0f * (float)w;
    float texPosHeight = 1/54.0f  * (float)h;

    float left = 0, top = 0;
    unsigned int indOffset = 0;

    for (char letter : text) {
        if (letter == '\n') {
            top += h;
            left = 0;
            continue;
        }

        int baseIndex = (int)letter - 32;

        float texPosX = baseIndex%18 / 18.0f; //18 = Characters in row
        float texPosY = baseIndex/18 / 6.0f;  //6 = Number of columns

        auto letterVerts = std::vector<float> {
                left,     top,     0,  texPosX,               texPosY,                 0, 0, 0,
                left + w, top,     0,  texPosX + texPosWidth, texPosY,                 0, 0, 0,
                left + w, top + h, 0,  texPosX + texPosWidth, texPosY + texPosHeight,  0, 0, 0,
                left,     top + h, 0,  texPosX,               texPosY + texPosHeight,  0, 0, 0,
        };

        for (float f : letterVerts) textVertices->push_back(f);

        textIndices->push_back(    indOffset);
        textIndices->push_back(3 + indOffset);
        textIndices->push_back(1 + indOffset);
        textIndices->push_back(3 + indOffset);
        textIndices->push_back(2 + indOffset);
        textIndices->push_back(1 + indOffset);

        indOffset += 4;

        left += w;
    }

    Mesh* m = new Mesh();
    m->create(textVertices, textIndices);

    delete textVertices;
    delete textIndices;

    return m;
}

