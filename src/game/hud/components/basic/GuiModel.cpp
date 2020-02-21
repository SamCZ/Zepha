//
// Created by aurailus on 2019-12-17.
//

#include "GuiModel.h"

#include "../../../../def/ClientGame.h"

GuiModel::GuiModel(const std::string &key) : GuiComponent(key) {}

std::shared_ptr<GuiModel> GuiModel::fromSerialized(SerialGui::Elem s, ClientGame &game, glm::ivec2 bounds) {
    glm::vec2 pos        = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position", bounds);
    glm::vec2 scale      = SerialGui::deserializeToken<glm::vec2>(s.tokens, "scale");
    glm::vec2 anim_range = SerialGui::deserializeToken<glm::vec2>(s.tokens, "anim_range");
    if (scale == glm::vec2{0, 0}) scale = {1, 1};

    std::string type    = s.tokens.count("type") ? s.tokens["type"] : "model";
    std::string source  = s.tokens["source"];
    std::string texture = s.tokens["texture"];

    auto m = std::make_shared<Model>();
    if (type == "model") m->fromSerialized(game.models.models[source], {game.textures[texture]});

    auto model = std::make_shared<GuiModel>(s.key);
    model->create(scale, m);
    model->setPos(pos);

    if (anim_range.y != 0) model->animate(anim_range);

    return model;
}

void GuiModel::create(glm::vec2 scale, std::shared_ptr<Model> model) {
    entity.setModel(model);
    setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});

    setRotationX(180);
    setRotationY(215);
}

void GuiModel::update(double delta) {
    entity.update(delta);
}

void GuiModel::animate(glm::vec2 range) {
    entity.playRange(range.x, range.y, true);
}

void GuiModel::setRotationX(float x) {
    entity.setRotateX(x);
}

void GuiModel::setRotationY(float y) {
    entity.setRotateY(y);
}

void GuiModel::setRotationZ(float z) {
    entity.setRotateZ(z);
}

void GuiModel::draw(Renderer &renderer) {
    renderer.toggleDepthTest(true);
    renderer.clearDepthBuffer();
    GuiComponent::draw(renderer);
    renderer.toggleDepthTest(false);
}
