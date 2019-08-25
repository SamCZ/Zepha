//
// Created by aurailus on 02/03/19.
//

#pragma once


#include <glm/vec3.hpp>
#include "FrustumPlane.h"
#include "FrustumAABB.h"

class Frustum {
private:
    enum {
        TOP = 0,
        BOTTOM,
        LEFT,
        RIGHT,
        NEAR,
        FAR
    };

public:
    enum {
        OUTSIDE,
        INSIDE,
        INTERSECT
    };

    Frustum() = default;

    void setCamInternals(float angle, float ratio, float nearD, float farD);
    void update(glm::vec3 &pos, glm::vec3 &look, glm::vec3 &up, glm::vec3 &right);

    int pointInFrustum(glm::vec3 &p);
    int boxInFrustum(FrustumAABB &b);

    FrustumPlane planes[6];

    float nearD, farD;
    float nearW, nearH, farW, farH;

    glm::vec3 ntl, ntr, nbl, nbr,
              ftl, ftr, fbl, fbr;
};

