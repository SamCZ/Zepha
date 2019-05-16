//
// Created by aurailus on 10/02/19.
//

#include "TextureRect.h"

TextureRect::TextureRect(glm::vec4 color) {
    createColored(color, color, color, color);
}

TextureRect::TextureRect(glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br) {
    createColored(tl, tr, bl, br);
}

TextureRect::TextureRect(Texture *texture) {
    createTextured(texture);
}

void TextureRect::createColored(glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br) {
    std::vector<Vertex> vertices {
        {{0, 0, 0}, 0, {tl.x, tl.y, tl.z, tl.w}, {0, 0, 0}},
        {{0, 1, 0}, 0, {bl.x, bl.y, bl.z, bl.w}, {0, 0, 0}},
        {{1, 1, 0}, 0, {br.x, br.y, br.z, br.w}, {0, 0, 0}},
        {{1, 0, 0}, 0, {tr.x, tr.y, tr.z, tr.w}, {0, 0, 0}},
    };
    std::vector<unsigned int> indices { 0, 1, 2, 2, 3, 0 };

    auto m = new Mesh();
    m->create(vertices, indices);

    setMesh(m);
}


void TextureRect::createTextured(Texture *texture) {
    std::vector<Vertex> vertices {
        {{0, 0, 0}, 1, {0, 0, 0, 0}, {0, 0, 0}},
        {{0, 1, 0}, 1, {0, 1, 0, 0}, {0, 0, 0}},
        {{1, 1, 0}, 1, {1, 1, 0, 0}, {0, 0, 0}},
        {{1, 0, 0}, 1, {1, 0, 0, 0}, {0, 0, 0}},
    };
    std::vector<unsigned int> indices { 0, 1, 2, 2, 3, 0 };

    auto m = new Mesh();
    m->create(vertices, indices);

    setMesh(m, texture);
}

void TextureRect::setTexture(Texture *texture) {
    createTextured(texture);
}


