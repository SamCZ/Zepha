//
// Created by aurailus on 08/02/19.
//

#ifndef ZEUS_HISTOGRAM_H
#define ZEUS_HISTOGRAM_H


#include "../Entity.h"

class Histogram : public Entity {
public:
    Histogram() = default;
    Histogram(Texture *texture, int length, float maxVal, bool editInPlace);

    void push_back(float value);
private:
    Mesh* buildHistogramMesh();

    float maxVal = 0;
    bool dynamicMax = true;

    int length = 60;
    int insertionPoint = 0;

    bool editInPlace = false;

    std::vector<float> history;
};


#endif //ZEUS_HISTOGRAM_H