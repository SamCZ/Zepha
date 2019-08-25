//
// Created by aurailus on 28/04/19.
//

#pragma once

#include "../../../world/Entity.h"
#include "../../../../../util/Util.h"
#include "../basic/GUIRect.h"
#include "../basic/GUIGraph.h"
#include "../basic/GUIText.h"
#include "../basic/GUIContainer.h"

class GUILabelledGraph : public GUIContainer {
public:
    GUILabelledGraph() = default;
    GUILabelledGraph(const std::string& key);

    void create(glm::vec2 scale, glm::vec4 padding, const std::string& title,
                unsigned int graphLength, unsigned int graphScale,
                std::shared_ptr<AtlasRef> graphTextureRef, Font font);

    void pushValue(float value);
private:
    const static int GRAPH_PAD_X = 2, GRAPH_PAD_Y = 62, TEXT_PAD_X = 4, TEXT_PAD_Y = 8;

    std::string title;

    std::shared_ptr<AtlasRef> graphTextureRef;
    Font font;

    int ind = 0;
    float history[5] {};
};

