//
// Created by aurailus on 2019-12-17.
//

#include "GuiModel.h"

#include "client/gui/SerialGui.h"
#include "client/graph/Model.h"
#include "client/graph/Renderer.h"
#include "game/LocalSubgame.h"
#include "game/atlas/asset/ModelStore.h"
#include "game/atlas/TextureAtlas.h"

GuiModel::GuiModel(const std::string &key) : GuiComponent(key) {}

std::shared_ptr<GuiModel> GuiModel::fromSerialized(const LuaGuiElement& elem, TextureAtlas& textures, ModelStore& models, glm::ivec2 bounds) {
    glm::vec2 pos        = SerialGui::get<glm::vec2>(elem, "position", bounds);
    glm::vec2 scale      = SerialGui::get<glm::vec2>(elem, "scale");
    glm::vec2 anim_range = SerialGui::get<glm::vec2>(elem, "anim_range");
    if (scale == glm::vec2{0, 0}) scale = {1, 1};

    std::string type    = elem.get_or<std::string>("type", "model");
    std::string source  = elem.get_or<std::string>("source", "");
    std::string texture = elem.get_or<std::string>("texture", "");

    auto m = std::make_shared<Model>();
    if (type == "model") m->fromSerialized(models.models[source], {textures[texture]});

    auto model = std::make_shared<GuiModel>(elem.key);
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
    entity.animation.setAnim(range, 0, true);
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
