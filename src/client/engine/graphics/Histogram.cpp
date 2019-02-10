//
// Created by aurailus on 08/02/19.
//

#include "Histogram.h"

Histogram::Histogram(Texture *texture, int length, float maxVal, bool editInPlace) {
    this->length = length;
    this->maxVal = maxVal;
    this->editInPlace = editInPlace;

    this->history = std::vector<float>((unsigned long)length);
    this->insertionPoint = 0;

    setTexture(texture);
    mesh = new Mesh();
}

void Histogram::push_back(float value) {
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

    cleanup();
    delete mesh;

    Mesh* mesh = buildHistogramMesh();
    create(mesh);
}

Mesh* Histogram::buildHistogramMesh() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    unsigned int indOffset = 0;
    float xOffset = 0;
    float i = 0.10;

    for (float num : history) {
        float distFromPointer = (xOffset <= insertionPoint) ? insertionPoint - xOffset : insertionPoint + 120 - xOffset;

        float h = num / maxVal;
        float section = std::round(9 - (float)fmax(maxVal - num, 0.0)/6) / 10.0f;
        float age = std::round((90 - (distFromPointer / length)*90)) / 100.0f;

        auto columnVerts = std::vector<float> {
            xOffset,     -h, 0, age,       section,   0, 0, 0,
            xOffset + 1, -h, 0, age+0.01f, section,   0, 0, 0,
            xOffset + 1,  0, 0, age+0.01f, section+i, 0, 0, 0,
            xOffset,      0, 0, age,       section+i, 0, 0, 0,
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