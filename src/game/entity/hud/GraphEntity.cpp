//
// Created by aurailus on 08/02/19.
//

#include "GraphEntity.h"

GraphEntity::GraphEntity(Texture *texture, int length, float maxVal, bool editInPlace) :
    length(length),
    maxVal(maxVal),
    dynamicMax(maxVal <= 0),
    editInPlace(editInPlace),
    history(std::vector<float>(static_cast<unsigned long>(length))) {

    setTexture(texture);
    setMesh(new EntityMesh());
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

    EntityMesh* mesh = buildHistogramMesh();
    setMesh(mesh);
}

EntityMesh* GraphEntity::buildHistogramMesh() {
    std::vector<EntityVertex> vertices;
    std::vector<unsigned int> indices;

    unsigned int indOffset = 0;
    float xOffset = 0;

    for (float num : history) {
        float distFromPointer = (xOffset <= insertionPoint) ? insertionPoint - xOffset : insertionPoint + length - xOffset;
        float age = std::round((90 - (distFromPointer / length)*90)) / 100.0f;

        float h = num / maxVal;
        float sec = (float)std::round(9 - fmin(h, 1)*9) * 0.1f;

        auto columnVerts = std::vector<EntityVertex> {
            {{xOffset,    -h, 0}, {age,       sec,       0, 0}, true, {}},
            {{xOffset + 1,-h, 0}, {age+0.01f, sec,       0, 0}, true, {}},
            {{xOffset + 1, 0, 0}, {age+0.01f, sec+0.10f, 0, 0}, true, {}},
            {{xOffset,     0, 0}, {age,       sec+0.10f, 0, 0}, true, {}},
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

    auto m = new EntityMesh();
    m->create(vertices, indices);

    return m;
}

void GraphEntity::setMax(float max) {
    maxVal = max;
}
