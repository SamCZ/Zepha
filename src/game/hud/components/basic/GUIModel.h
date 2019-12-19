//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <string>
#include "GUIContainer.h"
#include "../../../../def/ItemDef.h"
#include "../../../../util/Pointer.h"

class GUIModel : public GUIComponent {
public:
    GUIModel() = default;
    GUIModel(const std::string& key);

    void create(glm::vec2 scale, sptr<GuiMesh> model);

    void setRotation(glm::mat4 rotation);

    void draw(Renderer& renderer) override;

    sptr<GuiMesh> model;
};
