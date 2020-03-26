//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <string>

#include "GuiContainer.h"
#include "../../../../def/ItemDef.h"
#include "../../SerialGui.h"

class ClientGame;
class ModelStore;

class GuiModel : public GuiComponent {
public:
    GuiModel() = default;
    GuiModel(const std::string& key);

    static std::shared_ptr<GuiModel> fromSerialized(const SerialGui::Element& elem, TextureAtlas& textures, ModelStore& models, glm::ivec2 bounds);

    void create(glm::vec2 scale, std::shared_ptr<Model> model);
    void update(double delta) override;

    void animate(glm::vec2 range);

    void setRotationX(float x);
    void setRotationY(float x);
    void setRotationZ(float x);

    void draw(Renderer& renderer) override;
protected:
    float depth = 300;
};
