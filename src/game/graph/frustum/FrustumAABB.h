//
// Created by aurailus on 03/03/19.
//

#ifndef ZEUS_FRUSTUMAABB_H
#define ZEUS_FRUSTUMAABB_H


#include <glm/vec3.hpp>

class FrustumAABB {
public:
    glm::vec3 corner;
    glm::vec3 size;

    FrustumAABB(glm::vec3 a, glm::vec3 b);

    void set(glm::vec3 a, glm::vec3 b);

    glm::vec3 getVertexP(glm::vec3 &normal);
    glm::vec3 getVertexN(glm::vec3 &normal);
};


#endif //ZEUS_FRUSTUMAABB_H
