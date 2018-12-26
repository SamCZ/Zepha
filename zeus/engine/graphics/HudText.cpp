#include <utility>

//
// Created by aurailus on 25/12/18.
//

#include "HudText.h"
#include "../../mesh/TextBuilder.h"

HudText::HudText() {
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