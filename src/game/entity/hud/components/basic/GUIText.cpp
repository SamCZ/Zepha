#include <utility>


//
// Created by aurailus on 25/12/18.
//

#include "GUIText.h"

GUIText::GUIText(const std::string &key) : GUIComponent(key) {}

void GUIText::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 bgcolor, glm::vec4 color, std::shared_ptr<AtlasRef> fontRef) {
    // Text Constructor
    // Creates a GUIText object.

    this->scale = scale;
    this->padding = padding;
    this->fontRef = std::move(fontRef);

    this->bgcolor = bgcolor;
    //TODO: Text Color

    entity.setScale({scale.x, scale.y, 1});
    setText("");
}

void GUIText::setText(std::string text) {
    const static int w = 7;
    const static int h = 9;

    this->text = std::move(text);

    std::vector<EntityVertex> textVertices;
    std::vector<unsigned int> textIndices;

    float texPosWidth  = 1/128.0f * (float)w;
    float texPosHeight = 1/54.0f  * (float)h;

    float left = 0, top = 0;
    unsigned int indOffset = 0;

    if (bgcolor.w != 0) {
        float width = 1;
        float up = 0;

        for (int ind = 0; ind < this->text.length(); ind++) {
            char letter = this->text[ind];

            if (letter == '\n' || ind == this->text.length() - 1) {
                if (this->text.length() - 1 == ind) {
                    width += w - getWidthSubtract((int)letter - 32) * 2;
                }

                if (width > 1) {
                    std::vector<EntityVertex> vertices {
                        {{-1,    -1 + up,     0}, bgcolor, false},
                        {{-1,     h + 1 + up, 0}, bgcolor, false},
                        {{width,  h + 1 + up, 0}, bgcolor, false},
                        {{width, -1 + up,     0}, bgcolor, false},
                    };
                    std::vector<unsigned int> indices {
                        0 + indOffset,
                        1 + indOffset,
                        2 + indOffset,
                        2 + indOffset,
                        3 + indOffset,
                        0 + indOffset
                    };

                    textVertices.insert(textVertices.end(), vertices.begin(), vertices.end());
                    textIndices.insert(textIndices.end(), indices.begin(), indices.end());

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

        auto uv = fontRef->uv;
        uv.z -= uv.x;
        uv.w -= uv.y;

        auto letterVerts = std::vector<EntityVertex> {
             {{left,             top,     0}, {uv.x + texPosL * uv.z, uv.y + texPosY * uv.w,                 0, 0}, true, {}},
             {{left + w - p * 2, top,     0}, {uv.x + texPosR * uv.z, uv.y + texPosY * uv.w,                 0, 0}, true, {}},
             {{left + w - p * 2, top + h, 0}, {uv.x + texPosR * uv.z, uv.y + (texPosY+texPosHeight) * uv.w,  0, 0}, true, {}},
             {{left,             top + h, 0}, {uv.x + texPosL * uv.z, uv.y + (texPosY+texPosHeight) * uv.w,  0, 0}, true, {}},
        };

        textVertices.insert(textVertices.end(), letterVerts.begin(), letterVerts.end());

        textIndices.push_back(    indOffset);
        textIndices.push_back(3 + indOffset);
        textIndices.push_back(1 + indOffset);
        textIndices.push_back(3 + indOffset);
        textIndices.push_back(2 + indOffset);
        textIndices.push_back(1 + indOffset);

        indOffset += 4;

        left += w - p * 2;
    }

    auto m = new EntityMesh();
    m->create(textVertices, textIndices);
    entity.setMesh(m);
}

int GUIText::getWidthSubtract(int c) {
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

std::string GUIText::getText() {
    return text;
}
