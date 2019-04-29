#include <utility>

//
// Created by aurailus on 25/12/18.
//

#include "TextEntity.h"


TextEntity::TextEntity(Texture *texture, bool background, int scale) :
    background(background) {

    setTexture(texture);
    set("");
    setScale({2, 2, 1});
}

void TextEntity::set(std::string text) {
    this->text = std::move(text);

    std::vector<float> textVertices;
    std::vector<unsigned int> textIndices;

    float texPosWidth  = 1/128.0f * (float)w;
    float texPosHeight = 1/54.0f  * (float)h;

    float left = 0, top = 0;
    unsigned int indOffset = 0;

    if (background) {
        float width = 1;
        float up = 0;

        for (int ind = 0; ind < this->text.length(); ind++) {
            char letter = this->text[ind];

            if (letter == '\n' || ind == this->text.length() - 1) {
                if (this->text.length() - 1 == ind) {
                    width += w - getWidthSubtract((int)letter - 32) * 2;
                }

                if (width > 1) {
                    std::vector<float> vertices{
                            -1, -1 + up, 0, 0, 0.1, 0.1, 0.1, 0.3, 0, 0, 0,
                            -1, h + 1 + up, 0, 0, 0.1, 0.1, 0.1, 0.3, 0, 0, 0,
                            width, h + 1 + up, 0, 0, 0.1, 0.1, 0.1, 0.3, 0, 0, 0,
                            width, -1 + up, 0, 0, 0.1, 0.1, 0.1, 0.3, 0, 0, 0,
                    };
                    std::vector<unsigned int> indices{
                            0 + indOffset,
                            1 + indOffset,
                            2 + indOffset,
                            2 + indOffset,
                            3 + indOffset,
                            0 + indOffset
                    };

                    for (auto v : vertices) {
                        textVertices.push_back(v);
                    }
                    for (auto i : indices) {
                        textIndices.push_back(i);
                    }

                    indOffset += 4;
                    up += h + 2;
                }
                else {
                    up += h / 2;
                }

                width = 0;
            }
            else {
                width += w - getWidthSubtract((int)letter - 32) * 2;
            }
        }
    }

    int lineLength = 0;

    for (char letter : this->text) {
        if (letter == '\n') {
            top += (lineLength > 0) ? h + 2 : h / 2;
            lineLength = 0;
            left = 0;
            continue;
        }

        lineLength++;

        int baseIndex = (int)letter - 32;
        int p = getWidthSubtract(baseIndex);

        float texPosX = baseIndex%18 / 18.0f; //18 = Characters in row
        float texPosY = baseIndex/18 / 6.0f;  //6 = Number of columns

        float texPosL = texPosX + (p / 128.0f);
        float texPosR = texPosX + texPosWidth - ((p / 128.0f));

        auto letterVerts = std::vector<float> {
                left,             top,     0, 1,  texPosL, texPosY,                 0, 0, 0, 0, 0,
                left + w - p * 2, top,     0, 1,  texPosR, texPosY,                 0, 0, 0, 0, 0,
                left + w - p * 2, top + h, 0, 1,  texPosR, texPosY + texPosHeight,  0, 0, 0, 0, 0,
                left,             top + h, 0, 1,  texPosL, texPosY + texPosHeight,  0, 0, 0, 0, 0,
        };

        for (float f : letterVerts) textVertices.push_back(f);

        textIndices.push_back(    indOffset);
        textIndices.push_back(3 + indOffset);
        textIndices.push_back(1 + indOffset);
        textIndices.push_back(3 + indOffset);
        textIndices.push_back(2 + indOffset);
        textIndices.push_back(1 + indOffset);

        indOffset += 4;

        left += w - p * 2;
    }

    Mesh* m = new Mesh();
    m->create(&textVertices, &textIndices);
    setMesh(m);
}

int TextEntity::getWidthSubtract(int c) {
    switch (c) {
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
            return 2;
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
            return 1;
            break;
        default:
            break;
    }
    return 0;
}
