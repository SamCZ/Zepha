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

    entity.setScale(scale);

    setText("");
}

void GUIText::setText(std::string text) {
    this->text = std::move(text);
    unsigned int indOffset = 0;

    maxLineWidth = 0;

    std::vector<GuiVertex> textVertices;
    textVertices.reserve(text.length()*8 + 200);
    std::vector<unsigned int> textIndices;
    textIndices.reserve(text.length()*12 + 240);

    //Draw background & Measure Line Width
    int lineWidth = 0;
    int xOffset = 0, yOffset = 0;
    int h = Font::CHAR_HEIGHT;

    for (unsigned int i = 0; i < this->text.length() + 1; i++) {
        char c = this->text[i];

        if (c == '\n' || i == this->text.length()) {
            if (lineWidth > 0) {
                lineWidth += 2;

                if (lineWidth > maxLineWidth) maxLineWidth = lineWidth;

                if (bgcolor.w != 0) {
                    textVertices.emplace_back(glm::vec2{-1,            yOffset - 1    }, bgcolor, glm::vec3{1, 1, 1}, 0.f);
                    textVertices.emplace_back(glm::vec2{-1,            yOffset + h + 1}, bgcolor, glm::vec3{1, 1, 1}, 0.f);
                    textVertices.emplace_back(glm::vec2{lineWidth + 1, yOffset + h + 1}, bgcolor, glm::vec3{1, 1, 1}, 0.f);
                    textVertices.emplace_back(glm::vec2{lineWidth + 1, yOffset - 1    }, bgcolor, glm::vec3{1, 1, 1}, 0.f);

                    textIndices.emplace_back(indOffset);
                    textIndices.emplace_back(indOffset + 1);
                    textIndices.emplace_back(indOffset + 2);
                    textIndices.emplace_back(indOffset + 2);
                    textIndices.emplace_back(indOffset + 3);
                    textIndices.emplace_back(indOffset);

                    indOffset += 4;
                }
                yOffset += h + 2;
            }
            else {
                yOffset += h / 2; //Pad out the height if using just newlines.
            }

            lineWidth = 0;
        }
        else lineWidth += font.getCharWidth(c) + 1;
    }

    //Draw Characters

    bool emptyLine = true;
    xOffset = 0;
    yOffset = 0;

    for (unsigned int i = 0; i < this->text.length() + 1; i++) {
        char c = this->text[i];
        unsigned int h = Font::CHAR_HEIGHT;

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

        for (unsigned int j = 0; j <= 1; j++) {
            glm::vec3 c = {this->color.x, this->color.y, this->color.z};

            if (j == 0) {
                c *= glm::vec3 {0.4, 0.4, 0.45};
                xOffset += 1;
                yOffset += 1;
            }
            else {
                xOffset -= 1;
                yOffset -= 1;
            }

            textVertices.emplace_back(glm::vec2 {xOffset,             yOffset    }, glm::vec4 {charUVs.x, charUVs.y, 0, color.w}, c, 1.f);
            textVertices.emplace_back(glm::vec2 {xOffset,             yOffset + h}, glm::vec4 {charUVs.x, charUVs.w, 0, color.w}, c, 1.f);
            textVertices.emplace_back(glm::vec2 {xOffset + charWidth, yOffset + h}, glm::vec4 {charUVs.z, charUVs.w, 0, color.w}, c, 1.f);
            textVertices.emplace_back(glm::vec2 {xOffset + charWidth, yOffset    }, glm::vec4 {charUVs.z, charUVs.y, 0, color.w}, c, 1.f);

            textIndices.emplace_back(indOffset);
            textIndices.emplace_back(indOffset + 1);
            textIndices.emplace_back(indOffset + 2);
            textIndices.emplace_back(indOffset + 2);
            textIndices.emplace_back(indOffset + 3);
            textIndices.emplace_back(indOffset);

            indOffset += 4;
        }

        xOffset += charWidth;
    }

    auto m = new GuiMesh();
    m->create(textVertices, textIndices);
    entity.setMesh(m);
}

std::string GUIText::getText() {
    return text;
}

unsigned int GUIText::getWidth() {
    return maxLineWidth;
}
