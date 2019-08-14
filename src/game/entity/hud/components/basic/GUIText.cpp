//
// Created by aurailus on 25/12/18.
//

#include "GUIText.h"
#include <utility>

GUIText::GUIText(const std::string &key) : GUIComponent(key) {}

void GUIText::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 bgcolor, glm::vec4 color, Font font) {
    // Text Constructor
    // Creates a GUIText object.

    this->scale = scale;
    this->padding = padding;
    this->font = std::move(font);

    this->bgcolor = bgcolor;
    this->color = color;

    entity.setScale({scale.x, scale.y, 1});

    setText("");
}

void GUIText::setText(std::string text) {
    this->text = std::move(text);
    uint indOffset = 0;

    std::vector<EntityVertex> textVertices;
    std::vector<unsigned int> textIndices;

    //Create background rectangles
    if (bgcolor.w != 0) {
        int lineWidth = 0;
        int yOffset = 0;
        int h = Font::CHAR_HEIGHT;

        for (uint i = 0; i < this->text.length() + 1; i++) {
            char c = this->text[i];

            if (c == '\n' || i == this->text.length()) {
                if (lineWidth > 0) {
                    lineWidth += 2;

                    std::vector<EntityVertex> vertices {
                        {{-1,            yOffset - 1,     0}, bgcolor, {1, 1, 1}, false, {}},
                        {{-1,            yOffset + h + 1, 0}, bgcolor, {1, 1, 1}, false, {}},
                        {{lineWidth + 1, yOffset + h + 1, 0}, bgcolor, {1, 1, 1}, false, {}},
                        {{lineWidth + 1, yOffset - 1,     0}, bgcolor, {1, 1, 1}, false, {}},
                    };
                    std::vector<uint> indices {
                        indOffset,
                        indOffset + 1,
                        indOffset + 2,
                        indOffset + 2,
                        indOffset + 3,
                        indOffset
                    };

                    textVertices.insert(textVertices.end(), vertices.begin(), vertices.end());
                    textIndices.insert(textIndices.end(), indices.begin(), indices.end());

                    indOffset += 4;
                    yOffset += h + 2;
                }
                else {
                    yOffset += h / 2; //Pad out the height if using just newlines.
                }

                lineWidth = 0;
            }
            else {
                lineWidth += font.getCharWidth(c) + 1;
            }
        }
    }

    //Print the characters
    uint xOffset = 0;
    uint yOffset = 0;

    bool emptyLine = true;

    for (uint i = 0; i < this->text.length() + 1; i++) {
        char c = this->text[i];
        uint h = Font::CHAR_HEIGHT;

        if (c == '\n' || i == this->text.length()) {
            yOffset += (emptyLine) ? h / 2 : h + 2;
            xOffset = 0;
            emptyLine = true;
            continue;
        }
        else {
            emptyLine = false;
        }

        auto charWidth = font.getCharWidth(c) + 1;
        auto charUVs = font.getCharUVs(c);

        for (uint j = 0; j <= 1; j++) {
            glm::vec3 color = {this->color.x, this->color.y, this->color.z};

            if (j == 0) {
                color *= glm::vec3 {0.4, 0.4, 0.45};
                xOffset += 1;
                yOffset += 1;
            }
            else {
                xOffset -= 1;
                yOffset -= 1;
            }

            std::vector<EntityVertex> vertices{
                    {{xOffset,             yOffset,     0}, {charUVs.x, charUVs.y, 0, 0}, color, true, {}},
                    {{xOffset,             yOffset + h, 0}, {charUVs.x, charUVs.w, 0, 0}, color, true, {}},
                    {{xOffset + charWidth, yOffset + h, 0}, {charUVs.z, charUVs.w, 0, 0}, color, true, {}},
                    {{xOffset + charWidth, yOffset,     0}, {charUVs.z, charUVs.y, 0, 0}, color, true, {}},
            };
            std::vector<uint> indices{
                    indOffset,
                    indOffset + 1,
                    indOffset + 2,
                    indOffset + 2,
                    indOffset + 3,
                    indOffset
            };

            textVertices.insert(textVertices.end(), vertices.begin(), vertices.end());
            textIndices.insert(textIndices.end(), indices.begin(), indices.end());

            indOffset += 4;
        }

        xOffset += charWidth;
    }

    auto m = new EntityMesh();
    m->create(textVertices, textIndices);
    entity.setMesh(m);
}

std::string GUIText::getText() {
    return text;
}
