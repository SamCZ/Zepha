#include <utility>

//
// Created by aurailus on 25/12/18.
//

#include "HudText.h"

HudText::HudText(Texture *texture) {
    setTexture(texture);
    set("");
}

void HudText::set(std::string text) {
    this->text = std::move(text);
    Mesh* mesh = TextBuilder().build(this->text);
    setMesh(mesh);
}
