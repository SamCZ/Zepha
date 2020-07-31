//
// Created by aurailus on 28/04/19.
//

#pragma once

#include "../../../entity/DrawableEntity.h"
#include "../../../../util/Util.h"
#include "../basic/GuiRect.h"
#include "../basic/GuiGraph.h"
#include "../basic/GuiText.h"
#include "../basic/GuiContainer.h"

class GuiLabelledGraph : public GuiContainer {
public:
    GuiLabelledGraph() = default;
    GuiLabelledGraph(const std::string& key);

    void create(glm::vec2 scale, glm::vec4 padding, const std::string& title,
                unsigned int graphLength, unsigned int graphScale,
                std::shared_ptr<AtlasRef> graphTextureRef, Font font);

    void pushValue(float value);
private:
    std::string title;

    std::shared_ptr<AtlasRef> graphTextureRef;
    Font font;

    int ind = 0;
    float history[5] {};
};

