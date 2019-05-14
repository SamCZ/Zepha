//
// Created by aurailus on 28/04/19.
//

#include "StatGraph.h"

StatGraph::StatGraph(std::string title, int graphLength, int graphScale, Texture *graphTex, Texture *textTex) {
    construct(std::move(title), 244, 64, graphLength, graphScale, graphTex, textTex);
}

StatGraph::StatGraph(std::string title, int xSize, int ySize, int graphLength, int graphScale, Texture* graphTex, Texture* textTex) {
    construct(std::move(title), xSize, ySize, graphLength, graphScale, graphTex, textTex);
}

void StatGraph::construct(std::string title, int xSize, int ySize, int graphLength, int graphScale, Texture *graphTex, Texture *textTex) {
    for (int i = 0; i < 10; i++) history[i] = 0;

    background = new TextureRect({0.1, 0.1, 0.1, 0.2}, {0.1, 0.1, 0.1, 0.2}, {0.1, 0.1, 0.1, 0.7}, {0.1, 0.1, 0.1, 0.7});
    background->setScale({xSize, ySize, 1});

    graph = new GraphEntity(graphTex, graphLength, graphScale, true);
    graph->setScale({(xSize - GRAPH_PAD_X * 2) / graphLength, 28, 1});

    this->title = std::move(title);
    text = new TextEntity(textTex);
    text->setScale(2);

    setPosition({0, 0});
}


void StatGraph::setPosition(glm::vec2 pos) {
    text->setPos({pos.x + TEXT_PAD_X, pos.y + TEXT_PAD_Y, 0});
    graph->setPos({pos.x + GRAPH_PAD_X, pos.y + GRAPH_PAD_Y, 0});
    background->setPos({pos.x, pos.y, 0});
}

void StatGraph::update(float value) {
    graph->push_back(value);

    history[ind] = value;
    if (++ind >= 10) {
        ind = 0;

        float val = 0;
        for (float i : history) val += i / 10;

        std::string stringVal = (value == static_cast<int>(value))
                                ? std::to_string(static_cast<int>(value))
                                : Util::floatToString(value);

        text->set(title + ": " + stringVal);
    }
}

void StatGraph::draw(Renderer &renderer) {
    if (isVisible()) {
        background->draw(renderer);
        graph->draw(renderer);
        text->draw(renderer);
    }
}