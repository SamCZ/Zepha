//
// Created by aurailus on 2019-12-11.
//

#include "GUIImageButton.h"

#include "../../../../def/ClientGame.h"
#include "../basic/GUIText.h"

GUIImageButton::GUIImageButton(const std::string &key) : GUIRect(key) {}

std::shared_ptr<GUIImageButton> GUIImageButton::fromSerialized(SerialGui::Elem s, ClientGame &game, glm::ivec2 bounds) {
    glm::vec2 pos     = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position", bounds);
    glm::vec2 offset  = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position_anchor");
    glm::vec2 size    = SerialGui::deserializeToken<glm::vec2>(s.tokens, "size", bounds);
    glm::vec4 padding = SerialGui::deserializeToken<glm::vec4>(s.tokens, "padding", bounds);

    pos -= offset * size;
    size -= glm::vec2 {padding.y + padding.w, padding.x + padding.z};

    std::string background = s.tokens["background"];
    std::string background_hover = s.tokens["background_hover"];
    if (background_hover.length() == 0) background_hover = background;

    std::string content = "";
    if (s.tokens.count("content") && s.tokens["content"].length() >= 2) content = s.tokens["content"].substr(1, s.tokens["content"].size() - 2);
    std::string::size_type off = 0;
    while ((off = content.find("\\n", off)) != std::string::npos) {
        content.replace(off, 2, "\n");
        off += 1;
    }

    auto button = std::make_shared<GUIImageButton>(s.key);
    button->create(size, padding, game.textures[background], game.textures[background_hover]);
    button->setPos(pos);

    if (content != "") {
        auto text = std::make_shared<GUIText>(s.key + "__TEXT");
        text->create(glm::vec2(SerialGui::SCALE_MODIFIER), padding, {}, {1, 1, 1, 1}, {game.textures, game.textures["font"]});
        text->setPos({6 * SerialGui::SCALE_MODIFIER, size.y / 2 - 4.5 * SerialGui::SCALE_MODIFIER});
        text->setText(content);
        button->add(text);
    }

    return button;
}

// Texture Constructor
// Creates a GUIImageButton object with two textures
// defined by the 'texture' & 'hoverTexture' reference.
void GUIImageButton::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, std::shared_ptr<AtlasRef> hoverTexture) {
    this->hoverTexture = hoverTexture;
    GUIRect::create(scale, padding, texture);

    setHoverCallback(nullptr);
}

void GUIImageButton::setHoverCallback(const callback& hoverCallback) {
    GUIComponent::setHoverCallback([&, hoverCallback](bool nowHovered, glm::ivec2 pos) {
        if (hoverCallback) hoverCallback(nowHovered, pos);
        if (nowHovered != hovered) this->rebuild(nowHovered);
    });
}

void GUIImageButton::rebuild(bool hover) {
    auto tex = (hover) ? (hoverTexture != nullptr) ? hoverTexture : texture : texture;

    auto mesh = std::make_unique<EntityMesh>();
    mesh->create({
        {{0, 0, 0}, {tex->uv.x, tex->uv.y, 0, 1}, {1, 1, 1}, true, {}, {}, {}},
        {{0, 1, 0}, {tex->uv.x, tex->uv.w, 0, 1}, {1, 1, 1}, true, {}, {}, {}},
        {{1, 1, 0}, {tex->uv.z, tex->uv.w, 0, 1}, {1, 1, 1}, true, {}, {}, {}},
        {{1, 0, 0}, {tex->uv.z, tex->uv.y, 0, 1}, {1, 1, 1}, true, {}, {}, {}}
    }, {0, 1, 2, 2, 3, 0});

    auto model = std::make_shared<Model>();
    model->fromMesh(std::move(mesh));

    entity.setModel(model);
    setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});
}