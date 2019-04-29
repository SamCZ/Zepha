//
// Created by aurailus on 08/02/19.
//

#ifndef ZEUS_HISTOGRAM_H
#define ZEUS_HISTOGRAM_H


#include "../Entity.h"

//TODO: Un-publicize entity

class GraphEntity : public Entity {
public:
    GraphEntity() = default;
    GraphEntity(Texture *texture, int length, float maxVal, bool editInPlace);

    void push_back(float value);
    void setMax(float max);
private:
    Mesh* buildHistogramMesh();

    bool dynamicMax = true;

    int length = 60;
    int insertionPoint = 0;

    float maxVal = 0;

    bool editInPlace = false;

    std::vector<float> history;
};


#endif //ZEUS_HISTOGRAM_H
