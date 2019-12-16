//
// Created by aurailus on 2019-12-11.
//

#pragma once

#include "../basic/GUIRect.h"

class GUIImageButton : public GUIRect {
public:
    GUIImageButton() = default;
    GUIImageButton(const std::string& key);

    void create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, std::shared_ptr<AtlasRef> hoverTexture);

    void setHoverCallback(const callback& hoverCallback) override;
private:
    void rebuild(bool hover);
    std::shared_ptr<AtlasRef> hoverTexture = nullptr;
};
