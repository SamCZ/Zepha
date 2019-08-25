//
// Created by aurailus on 08/02/19.
//

#include "GUIGraph.h"

GUIGraph::GUIGraph(const std::string &key) : GUIComponent(key) {}

void GUIGraph::create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr <AtlasRef> texture,
    unsigned int length, float maxValue, bool editInPlace) {

    this->scale = scale;
    this->padding = padding;

    entity.setScale({scale.x + padding.w + padding.y, scale.y + padding.x + padding.z});

    this->length = length;
    this->maxVal = maxValue;
    this->dynamicMax = maxValue <= 0;
    this->editInPlace = editInPlace;
    this->texture = std::move(texture);

    history = std::vector<float>(static_cast<unsigned long>(length));

    entity.setMesh(new GuiMesh());
}

void GUIGraph::pushValue(float value) {
    if (editInPlace) {
        insertionPoint++;
        if (insertionPoint >= length) insertionPoint = 0;
        history[insertionPoint] = value;
    }
    else {
        if (insertionPoint < length - 1) insertionPoint++;
        else {
            for (int i = 0; i < length; i++) {
                history[i] = history[i + 1];
            }
        }
        history[insertionPoint] = value;
    }
    if (dynamicMax) {
        maxVal = 0;
        for (float i : history) {
            if (i > maxVal) maxVal = i;
        }
    }

    buildHistogramMesh();
}

void GUIGraph::setMax(float max) {
    maxVal = max;
}

void GUIGraph::buildHistogramMesh() {
    std::vector<GuiVertex> vertices {};
    std::vector<unsigned int> indices {};

    auto uv = texture->uv;
    uv.z -= uv.x;
    uv.w -= uv.y;

    unsigned int indOffset = 0;
    float xOffset = 0;

    for (float num : history) {
        float distFromPointer = (xOffset <= insertionPoint) ? insertionPoint - xOffset : insertionPoint + length - xOffset;
        float age = std::round((90 - (distFromPointer / length)*90)) / 100.0f;

        float h = num / maxVal;
        float sec = (float)std::round(9 - fmin(h, 1)*9) * 0.1f;

        auto columnVerts = std::vector<GuiVertex> {
            {{xOffset,    -h}, {uv.x + age * uv.z,         uv.y + sec * uv.w,         0, 0}, {1, 1, 1}},
            {{xOffset + 1,-h}, {uv.x + (age+0.01f) * uv.z, uv.y + sec * uv.w,         0, 0}, {1, 1, 1}},
            {{xOffset + 1, 0}, {uv.x + (age+0.01f) * uv.z, uv.y + (sec+0.10f) * uv.w, 0, 0}, {1, 1, 1}},
            {{xOffset,     0}, {uv.x + age * uv.z,         uv.y + (sec+0.10f) * uv.w, 0, 0}, {1, 1, 1}},
        };

        vertices.insert(vertices.end(), columnVerts.begin(), columnVerts.end());

        indices.push_back(    indOffset);
        indices.push_back(3 + indOffset);
        indices.push_back(1 + indOffset);
        indices.push_back(3 + indOffset);
        indices.push_back(2 + indOffset);
        indices.push_back(1 + indOffset);

        xOffset ++;
        indOffset += 4;
    }

    auto m = new GuiMesh();
    m->create(vertices, indices);
    entity.setMesh(m);
}
