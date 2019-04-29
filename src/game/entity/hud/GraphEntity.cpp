//
// Created by aurailus on 08/02/19.
//

#include "GraphEntity.h"

GraphEntity::GraphEntity(Texture *texture, int length, float maxVal, bool editInPlace) {
    this->length = length;
    this->maxVal = maxVal;
    this->dynamicMax = (maxVal <= 0);

    this->editInPlace = editInPlace;

    this->history = std::vector<float>((unsigned long)length);
    this->insertionPoint = 0;

    setTexture(texture);
    setMesh(new Mesh());
}

void GraphEntity::push_back(float value) {
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

    Mesh* mesh = buildHistogramMesh();
    setMesh(mesh);
}

Mesh* GraphEntity::buildHistogramMesh() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int indOffset = 0;
    float xOffset = 0;

    for (float num : history) {
        float distFromPointer = (xOffset <= insertionPoint) ? insertionPoint - xOffset : insertionPoint + length - xOffset;
        float age = std::round((90 - (distFromPointer / length)*90)) / 100.0f;

        float h = num / maxVal;
        float sec = (float)std::round(9 - fmin(h, 1)*9) * 0.1f;

        auto columnVerts = std::vector<float> {
            xOffset,    -h, 0, 1, age,       sec,       0, 0, 0, 0, 0,
            xOffset + 1,-h, 0, 1, age+0.01f, sec,       0, 0, 0, 0, 0,
            xOffset + 1, 0, 0, 1, age+0.01f, sec+0.10f, 0, 0, 0, 0, 0,
            xOffset,     0, 0, 1, age,       sec+0.10f, 0, 0, 0, 0, 0,
        };

        for (float f : columnVerts) vertices.push_back(f);

        indices.push_back(    indOffset);
        indices.push_back(3 + indOffset);
        indices.push_back(1 + indOffset);
        indices.push_back(3 + indOffset);
        indices.push_back(2 + indOffset);
        indices.push_back(1 + indOffset);

        xOffset ++;
        indOffset += 4;
    }

    auto m = new Mesh();
    m->create(&vertices, &indices);

    return m;
}

void GraphEntity::setMax(float max) {
    maxVal = max;
}
