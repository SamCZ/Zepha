//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <string>
#include "GUIContainer.h"
#include "../../../../def/ItemDef.h"

class GUIModel : public GUIComponent {
public:
    GUIModel() = default;
    GUIModel(const std::string& key);

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
