//
// Created by aurailus on 2019-12-11.
//

#include "GUIImageButton.h"

GUIImageButton::GUIImageButton(const std::string &key) : GUIRect(key) {}

// Texture Constructor
// Creates a GUIImageButton object with two textures
// defined by the 'texture' & 'hoverTexture' reference.
void GUIImageButton::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, std::shared_ptr<AtlasRef> hoverTexture) {
    this->hoverTexture = hoverTexture;
    GUIRect::create(scale, padding, texture);

    setHoverCallback([&](bool hovered) { rebuild(hovered); });
}

void GUIImageButton::rebuild(bool hover) {
    auto tex = (hover) ? (hoverTexture != nullptr) ? hoverTexture : texture : texture;

    auto mesh = new GuiMesh();
    mesh->create({
        {{0, 0}, {tex->uv.x, tex->uv.y, 0, 1}, {1, 1, 1}, true},
        {{0, 1}, {tex->uv.x, tex->uv.w, 0, 1}, {1, 1, 1}, true},
        {{1, 1}, {tex->uv.z, tex->uv.w, 0, 1}, {1, 1, 1}, true},
        {{1, 0}, {tex->uv.z, tex->uv.y, 0, 1}, {1, 1, 1}, true}
    }, {0, 1, 2, 2, 3, 0});

    entity.setMesh(mesh);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});

}
