//
// Created by aurailus on 25/12/18.
//

#include <utility>

#include "GuiText.h"

GuiText::GuiText(const std::string &key) : GuiComponent(key) {}

void GuiText::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 bgcolor, glm::vec4 color, Font font) {
    // Text Constructor
    // Creates a GuiText object.

    this->scale = scale;
    this->padding = padding;
    this->font = std::move(font);

    this->bgcolor = bgcolor;
    this->color = color;

    setScale(scale);
    setText("");
}

std::shared_ptr<GuiText> GuiText::fromSerialized(SerialGui::Elem s, ClientGame &game, glm::ivec2 bounds) {
    glm::vec2 pos     = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position", bounds);
    glm::vec2 offset  = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position_anchor");
    glm::vec2 size    = SerialGui::deserializeToken<glm::vec2>(s.tokens, "size", bounds);
    glm::vec4 padding = SerialGui::deserializeToken<glm::vec4>(s.tokens, "padding", bounds);
    glm::vec2 scale =   SerialGui::deserializeToken<glm::vec2>(s.tokens, "scale");
    if (scale == glm::vec2{0, 0}) scale = {1, 1};

    pos -= offset * size;
//    size -= glm::vec2 {padding.y + padding.w, padding.x + padding.z};

    glm::vec4 background_color = Util::hexToColorVec("#0000");
    if (s.tokens.count("background")) background_color = Util::hexToColorVec(s.tokens["background"]);
    glm::vec4 color = Util::hexToColorVec("#fff");
    if (s.tokens.count("color")) color = Util::hexToColorVec(s.tokens["color"]);

    std::string content = "";
    if (s.tokens.count("content") && s.tokens["content"].length() >= 2) content = s.tokens["content"].substr(1, s.tokens["content"].size() - 2);
    std::string::size_type off = 0;
    while ((off = content.find("\\n", off)) != std::string::npos) {
        content.replace(off, 2, "\n");
        off += 1;
    }

    auto text = std::make_shared<GuiText>(s.key);
    text->create(scale * SerialGui::SCALE_MODIFIER, padding, background_color, color, {game.textures, game.textures["font"]});
    text->setText(content);
    text->setPos(pos);

    return text;
}

void GuiText::setText(std::string text) {
    this->text = std::move(text);
    unsigned int indOffset = 0;

    maxLineWidth = 0;

    std::vector<EntityVertex> textVertices;
    textVertices.reserve(text.length()*8 + 200);
    std::vector<unsigned int> textIndices;
    textIndices.reserve(text.length()*12 + 240);

    //Draw background & Measure Line Width
    int lineWidth = 0;
    int xOffset = 0, yOffset = 0;
    int h = Font::charHeight;

    for (unsigned int i = 0; i < this->text.length() + 1; i++) {
        char c = this->text[i];

        if (c == '\n' || i == this->text.length()) {
            if (lineWidth > 0) {
                lineWidth += 2;

                if (lineWidth > maxLineWidth) maxLineWidth = lineWidth;

                if (bgcolor.w != 0) {
                    textVertices.emplace_back(glm::vec3 {-1,            yOffset - 1,     0}, bgcolor, glm::vec3(1), 0.f, glm::vec3 {}, glm::ivec4 {}, glm::vec4 {});
                    textVertices.emplace_back(glm::vec3 {-1,            yOffset + h + 1, 0}, bgcolor, glm::vec3(1), 0.f, glm::vec3 {}, glm::ivec4 {}, glm::vec4 {});
                    textVertices.emplace_back(glm::vec3 {lineWidth + 1, yOffset + h + 1, 0}, bgcolor, glm::vec3(1), 0.f, glm::vec3 {}, glm::ivec4 {}, glm::vec4 {});
                    textVertices.emplace_back(glm::vec3 {lineWidth + 1, yOffset - 1,     0}, bgcolor, glm::vec3(1), 0.f, glm::vec3 {}, glm::ivec4 {}, glm::vec4 {});

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
        unsigned int h = Font::charHeight;

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

            textVertices.emplace_back(glm::vec3 {xOffset,             yOffset,     0}, glm::vec4 {charUVs.x, charUVs.y, 0, color.w}, c, 1.f, glm::vec3 {}, glm::ivec4 {}, glm::vec4 {});
            textVertices.emplace_back(glm::vec3 {xOffset,             yOffset + h, 0}, glm::vec4 {charUVs.x, charUVs.w, 0, color.w}, c, 1.f, glm::vec3 {}, glm::ivec4 {}, glm::vec4 {});
            textVertices.emplace_back(glm::vec3 {xOffset + charWidth, yOffset + h, 0}, glm::vec4 {charUVs.z, charUVs.w, 0, color.w}, c, 1.f, glm::vec3 {}, glm::ivec4 {}, glm::vec4 {});
            textVertices.emplace_back(glm::vec3 {xOffset + charWidth, yOffset,     0}, glm::vec4 {charUVs.z, charUVs.y, 0, color.w}, c, 1.f, glm::vec3 {}, glm::ivec4 {}, glm::vec4 {});

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

    auto m = std::make_unique<EntityMesh>();
    m->create(textVertices, textIndices);

    auto model = std::make_shared<Model>();
    model->fromMesh(std::move(m));

    entity.setModel(model);
}

std::string GuiText::getText() {
    return text;
}

unsigned int GuiText::getWidth() {
    return maxLineWidth;
}
