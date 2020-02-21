//
// Created by aurailus on 10/02/19.
//

#include "GuiRect.h"

#include "../../SerialGui.h"
#include "../../../../util/Util.h"
#include "../../../../def/ClientGame.h"

GuiRect::GuiRect(const std::string &key) : GuiComponent(key) {}

std::shared_ptr<GuiRect> GuiRect::fromSerialized(SerialGui::Elem s, ClientGame& game, glm::ivec2 bounds) {
    glm::vec2 pos     = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position", bounds);
    glm::vec2 offset  = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position_anchor");
    glm::vec2 size    = SerialGui::deserializeToken<glm::vec2>(s.tokens, "size", bounds);
    glm::vec4 padding = SerialGui::deserializeToken<glm::vec4>(s.tokens, "padding", bounds);

    pos -= offset * size;
    size -= glm::vec2 {padding.y + padding.w, padding.x + padding.z};

    std::string background = s.tokens["background"];

    bool hideOverflow = s.tokens["overflow"] == "hidden";

    auto rect = std::make_shared<GuiRect>(s.key);
    if (background[0] == '#') rect->create(size, padding, Util::hexToColorVec(background));
    else if (background.size() > 0) rect->create(size, padding, game.textures[background]);
    else rect->create(size, padding, glm::vec4 {});
    rect->setHideOverflow(hideOverflow);
    rect->setPos(pos);
    return rect;
}

// Single Color Constructor
// Creates a GuiRect object whose background
// is a flat color defined by 'color'.
void GuiRect::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 color) {
    this->scale = scale;
    this->padding = padding;

    auto mesh = std::make_unique<EntityMesh>();
    mesh->create({{{0, 0, 0}, color, {1, 1, 1}, false, {}, {}, {}},
                  {{0, 1, 0}, color, {1, 1, 1}, false, {}, {}, {}},
                  {{1, 1, 0}, color, {1, 1, 1}, false, {}, {}, {}},
                  {{1, 0, 0}, color, {1, 1, 1}, false, {}, {}, {}}
                  }, {0, 1, 2, 2, 3, 0});
    auto model = std::make_shared<Model>();
    model->fromMesh(std::move(mesh));

    entity.setModel(model);
    setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}

// Multiple Color Constructor
// Creates a GuiRect object with a gradient background
// defined by 'tl', 'tr', 'bl', and 'br'.
void GuiRect::create(glm::vec2 scale, glm::vec4 padding, glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br) {
    this->scale = scale;
    this->padding = padding;

    auto mesh = std::make_unique<EntityMesh>();
    mesh->create({
        {{0, 0, 0}, tl, {1, 1, 1}, false, {}, {}, {}},
        {{0, 1, 0}, bl, {1, 1, 1}, false, {}, {}, {}},
        {{1, 1, 0}, br, {1, 1, 1}, false, {}, {}, {}},
        {{1, 0, 0}, tr, {1, 1, 1}, false, {}, {}, {}}
    }, {0, 1, 2, 2, 3, 0});
    auto model = std::make_shared<Model>();
    model->fromMesh(std::move(mesh));

    entity.setModel(model);
    setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}

// Texture Constructor
// Creates a GuiRect object with a textured background
// defined by the 'texture' reference.
void GuiRect::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture) {
    this->scale = scale;
    this->padding = padding;
    this->texture = texture;
    this->hitbox = scale + glm::vec2{padding.y + padding.w, padding.x + padding.z};

    auto mesh = std::make_unique<EntityMesh>();
    mesh->create({
        {{0, 0, 0}, {this->texture->uv.x, this->texture->uv.y, 0, 1}, {1, 1, 1}, true, {}, {}, {}},
        {{0, 1, 0}, {this->texture->uv.x, this->texture->uv.w, 0, 1}, {1, 1, 1}, true, {}, {}, {}},
        {{1, 1, 0}, {this->texture->uv.z, this->texture->uv.w, 0, 1}, {1, 1, 1}, true, {}, {}, {}},
        {{1, 0, 0}, {this->texture->uv.z, this->texture->uv.y, 0, 1}, {1, 1, 1}, true, {}, {}, {}}
    }, {0, 1, 2, 2, 3, 0});
    auto model = std::make_shared<Model>();
    model->fromMesh(std::move(mesh));

    entity.setModel(model);
    setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}

// Texture Constructor
// Creates a GuiRect object with a textured background
// defined by the 'texture' reference.
void GuiRect::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, glm::vec4 tint) {
    this->scale = scale;
    this->padding = padding;
    this->texture = std::move(texture);

    auto mesh = std::make_unique<EntityMesh>();
    mesh->create({
        {{0, 0, 0}, {this->texture->uv.x, this->texture->uv.y, 0, tint.w}, glm::vec3{tint}, true, {}, {}, {}},
        {{0, 1, 0}, {this->texture->uv.x, this->texture->uv.w, 0, tint.w}, glm::vec3{tint}, true, {}, {}, {}},
        {{1, 1, 0}, {this->texture->uv.z, this->texture->uv.w, 0, tint.w}, glm::vec3{tint}, true, {}, {}, {}},
        {{1, 0, 0}, {this->texture->uv.z, this->texture->uv.y, 0, tint.w}, glm::vec3{tint}, true, {}, {}, {}}
    }, {0, 1, 2, 2, 3, 0});
    auto model = std::make_shared<Model>();
    model->fromMesh(std::move(mesh));

    entity.setModel(model);
    setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}
