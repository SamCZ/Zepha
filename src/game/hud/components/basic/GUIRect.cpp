//
// Created by aurailus on 10/02/19.
//

#include "GUIRect.h"

GUIRect::GUIRect(const std::string &key) : GUIComponent(key) {}

// Single Color Constructor
// Creates a GUIRect object whose background
// is a flat color defined by 'color'.
void GUIRect::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 color) {
    this->scale = scale;
    this->padding = padding;

    auto mesh = std::make_shared<GuiMesh>();
    mesh->create({{{0, 0, 0}, color, {1, 1, 1}, false}, {{0, 1, 0}, color, {1, 1, 1}, false},
                  {{1, 1, 0}, color, {1, 1, 1}, false}, {{1, 0, 0}, color, {1, 1, 1}, false}}, {0, 1, 2, 2, 3, 0});

    entity.setMesh(mesh);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}

// Multiple Color Constructor
// Creates a GUIRect object with a gradient background
// defined by 'tl', 'tr', 'bl', and 'br'.
void GUIRect::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br) {
    this->scale = scale;
    this->padding = padding;

    auto mesh = std::make_shared<GuiMesh>();
    mesh->create({{{0, 0, 0}, tl, {1, 1, 1}, false}, {{0, 1, 0}, bl, {1, 1, 1}, false},
                  {{1, 1, 0}, br, {1, 1, 1}, false}, {{1, 0, 0}, tr, {1, 1, 1}, false}}, {0, 1, 2, 2, 3, 0});

    entity.setMesh(mesh);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}

// Texture Constructor
// Creates a GUIRect object with a textured background
// defined by the 'texture' reference.
void GUIRect::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture) {
    this->scale = scale;
    this->padding = padding;
    this->texture = texture;
    this->hitbox = scale + glm::vec2{padding.y + padding.w, padding.x + padding.z};

    auto mesh = std::make_shared<GuiMesh>();
    mesh->create({
            {{0, 0, 0}, {this->texture->uv.x, this->texture->uv.y, 0, 1}, {1, 1, 1}, true},
            {{0, 1, 0}, {this->texture->uv.x, this->texture->uv.w, 0, 1}, {1, 1, 1}, true},
            {{1, 1, 0}, {this->texture->uv.z, this->texture->uv.w, 0, 1}, {1, 1, 1}, true},
            {{1, 0, 0}, {this->texture->uv.z, this->texture->uv.y, 0, 1}, {1, 1, 1}, true}
    }, {0, 1, 2, 2, 3, 0});

    entity.setMesh(mesh);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}

// Texture Constructor
// Creates a GUIRect object with a textured background
// defined by the 'texture' reference.
void GUIRect::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, glm::vec4 tint) {
    this->scale = scale;
    this->padding = padding;
    this->texture = std::move(texture);

    auto mesh = std::make_shared<GuiMesh>();
    mesh->create({
        {{0, 0, 0}, {this->texture->uv.x, this->texture->uv.y, 0, tint.w}, glm::vec3{tint}, true},
        {{0, 1, 0}, {this->texture->uv.x, this->texture->uv.w, 0, tint.w}, glm::vec3{tint}, true},
        {{1, 1, 0}, {this->texture->uv.z, this->texture->uv.w, 0, tint.w}, glm::vec3{tint}, true},
        {{1, 0, 0}, {this->texture->uv.z, this->texture->uv.y, 0, tint.w}, glm::vec3{tint}, true}
    }, {0, 1, 2, 2, 3, 0});

    entity.setMesh(mesh);
    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}
