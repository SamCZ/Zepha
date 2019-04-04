//
// Created by aurailus on 25/12/18.
//

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"

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
        int p = 0;

        switch (baseIndex) {
            case 1:
            case 7:
            case 12:
            case 14:
            case 26:
            case 27:
            case 64:
            case 73:
            case 74:
            case 76:
            case 109:
                p = 2;
                break;
            case 0:
            case 2:
            case 8:
            case 9:
            case 13:
            case 17:
            case 28:
            case 30:
            case 59:
            case 61:
            case 62:
            case 70:
            case 108:
            case 110:
                p = 1;
                break;
            default:
                break;
        }

        float texPosX = baseIndex%18 / 18.0f; //18 = Characters in row
        float texPosY = baseIndex/18 / 6.0f;  //6 = Number of columns

        float texPosL = texPosX + (p / 128.0f);
        float texPosR = texPosX + texPosWidth - ((p / 128.0f));

        auto letterVerts = std::vector<float> {
                left,             top,     0,  texPosL, texPosY,                 0, 0, 0,
                left + w - p * 2, top,     0,  texPosR, texPosY,                 0, 0, 0,
                left + w - p * 2, top + h, 0,  texPosR, texPosY + texPosHeight,  0, 0, 0,
                left,             top + h, 0,  texPosL, texPosY + texPosHeight,  0, 0, 0,
        };

        for (float f : letterVerts) textVertices->push_back(f);

        textIndices->push_back(    indOffset);
        textIndices->push_back(3 + indOffset);
        textIndices->push_back(1 + indOffset);
        textIndices->push_back(3 + indOffset);
        textIndices->push_back(2 + indOffset);
        textIndices->push_back(1 + indOffset);

        indOffset += 4;

        left += w - p * 2;
    }

    Mesh* m = new Mesh();
    m->create(textVertices, textIndices);

    delete textVertices;
    delete textIndices;

    return m;
}

#pragma clang diagnostic pop