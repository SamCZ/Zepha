//
// Created by aurailus on 03/03/19.
//

#include "FrustumAABB.h"

FrustumAABB::FrustumAABB(glm::vec3 a, glm::vec3 s) {
    corner = a;
    size = s;
}

void FrustumAABB::set(glm::vec3 a, glm::vec3 s) {
    corner = a;
    size = s;
}

glm::vec3 FrustumAABB::getVertexP(glm::vec3 &normal) {
    glm::vec3 res = corner;

    if (normal.x > 0) res.x += size.x;
    if (normal.y > 0) res.y += size.y;
    if (normal.z > 0) res.z += size.z;

    return res;
}

glm::vec3 FrustumAABB::getVertexN(glm::vec3 &normal){
    glm::vec3 res = corner;

    if (normal.x < 0) res.x += size.x;
    if (normal.y < 0) res.y += size.y;
    if (normal.z < 0) res.z += size.z;

    return res;
}


