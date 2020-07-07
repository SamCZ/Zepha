//
// Created by aurailus on 13/08/19.
//

#include <cmath>

#include "Font.h"

#include "AtlasRef.h"
#include "TextureAtlas.h"

Font::Font(TextureAtlas& atlas, std::shared_ptr<AtlasRef> tex) :
    fontTex(std::move(tex)),
    atlasSize(atlas.pixelSize) {

    getCharWidths(atlas);
}

unsigned int Font::getCharWidth(char c) {
    unsigned int index = static_cast<unsigned int>(c) - 32;
    if (index >= amountOfChars) throw "Invalid char index.";
    return charWidths[index];
}

void Font::getCharWidths(TextureAtlas &atlas) {
    auto& data = atlas.atlasData;

    charWidths[0] = 2;

    for (unsigned int i = 1; i < amountOfChars; i++) {
        glm::vec2 charPos = {i % 18 * charWidth, std::floor(i / 18) * charHeight};

        unsigned int xBase = static_cast<unsigned int>(fontTex->pos.x) + static_cast<unsigned int>(charPos.x);
        unsigned int yBase = static_cast<unsigned int>(fontTex->pos.y) + static_cast<unsigned int>(charPos.y);

        unsigned short width = 0;

        for (unsigned int j = 0; j < charWidth; j++) {
            bool empty = true;
            for (unsigned int k = 0; k < charHeight; k++) {
                unsigned int xx = xBase + j;
                unsigned int yy = yBase + k;

                unsigned int offset = yy * static_cast<unsigned int>(atlasSize.x) * 4 + xx * 4 + 3;

                if (data[offset] != 0) {
                    empty = false;
                    break;
                }
            }
            if (!empty) width = static_cast<unsigned short>(j);
        }

        charWidths[i] = width;
    }
}

glm::vec4 Font::getCharUVs(char c) {
    unsigned int index = static_cast<unsigned int>(c) - 32;
    if (index >= amountOfChars) throw "Invalid char index.";

    glm::vec2 charPos = {(index % 18) * charWidth, std::floor(index / 18) * charHeight};
    glm::vec4 uv = {
        fontTex->uv.x + (charPos.x) / atlasSize.x,
        fontTex->uv.y + (charPos.y) / atlasSize.y,
        fontTex->uv.x + (charPos.x + getCharWidth(c) + 1) / atlasSize.x,
        fontTex->uv.y + (charPos.y + charHeight) / atlasSize.y
    };

    return uv;
}
