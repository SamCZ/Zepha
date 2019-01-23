#include <utility>

//
// Created by aurailus on 25/12/18.
//

#include "HudText.h"

HudText::HudText() = default;

HudText::HudText(Texture *texture) {
    setTexture(texture);
    mesh = new Mesh();
    set("");
}

void HudText::set(std::string text) {
    this->text = std::move(text);
    cleanup();
    delete mesh;
    Mesh* mesh = TextBuilder().build(this->text);
    create(mesh);
}
