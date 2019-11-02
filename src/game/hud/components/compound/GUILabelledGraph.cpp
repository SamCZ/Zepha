//
// Created by aurailus on 28/04/19.
//

#include "GUILabelledGraph.h"

GUILabelledGraph::GUILabelledGraph(const std::string &key) : GUIContainer(key) {}

void GUILabelledGraph::create(glm::vec2 scale, glm::vec4 padding, const std::string &title,
    unsigned int graphLength, unsigned int graphScale,
    std::shared_ptr<AtlasRef> graphTextureRef, Font font) {

    const static int GRAPH_PAD_X = 2;
    const static int GRAPH_PAD_Y = 62;
    const static int TEXT_PAD_X = 4;
    const static int TEXT_PAD_Y = 8;

    this->scale = scale;
    this->padding = padding;
    this->title = title;
    this->font = std::move(font);
    this->graphTextureRef = std::move(graphTextureRef);

    auto background = std::make_shared<GUIRect>("background");
    background->create(scale, {}, {0.1, 0.1, 0.1, 0.2}, {0.1, 0.1, 0.1, 0.2}, {0.1, 0.1, 0.1, 0.7}, {0.1, 0.1, 0.1, 0.7});
    add(background);
    background->setPos({0, 0});

    auto graph = std::make_shared<GUIGraph>("graph");
    graph->create({scale.x / (graphLength + GRAPH_PAD_X), scale.y * 0.4}, {}, this->graphTextureRef, graphLength, graphScale, true);
    add(graph);
    graph->setPos({GRAPH_PAD_X, GRAPH_PAD_Y});

    auto label = std::make_shared<GUIText>("label");
    label->create({2, 2}, {}, {}, {1, 1, 1, 1}, this->font);
    add(label);
    label->setPos({TEXT_PAD_X, TEXT_PAD_Y});

    for (float &i : history) i = 0;
}

void GUILabelledGraph::pushValue(float value) {
    get<GUIGraph>("graph")->pushValue(value);

    history[ind] = value;
    if (++ind >= 5) {
        ind = 0;

        float val = 0;
        for (float i : history) val += i / 5;

        std::string stringVal = (value == static_cast<int>(value))
                                ? std::to_string(static_cast<int>(value))
                                : Util::floatToString(value);

        get<GUIText>("label")->setText(title + ": " + stringVal);
    }
}