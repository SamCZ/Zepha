//
// Created by aurailus on 28/04/19.
//

#ifndef ZEUS_STATGRAPH_H
#define ZEUS_STATGRAPH_H


#include "../Entity.h"
#include "TextureRect.h"
#include "GraphEntity.h"
#include "TextEntity.h"
#include "../../../util/Util.h"

class StatGraph : public Drawable {
public:
    StatGraph(std::string title, int xSize, int ySize, int graphLength, int graphScale, Texture* graphTex, Texture* textTex);
    StatGraph(std::string title, int graphLength, int graphScale, Texture* graphTex, Texture* textTex);

    void setPosition(glm::vec2 pos);
    void update(float value);

    void draw(Renderer& renderer) override;
private:
    void construct(std::string title, int xSize, int ySize, int graphLength, int graphScale, Texture* graphTex, Texture* textTex);

    const static int GRAPH_PAD_X = 2, GRAPH_PAD_Y = 62, TEXT_PAD_X = 4, TEXT_PAD_Y = 8;

    TextureRect* background;
    GraphEntity* graph;
    TextEntity* text;

    std::string title;
};


#endif //ZEUS_STATGRAPH_H
