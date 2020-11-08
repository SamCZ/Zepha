//
// Created by auri on 2020-11-07.
//

#include "Bounds.h"

Bounds::Bounds(glm::vec3 a, glm::vec3 b) : a(a), b(b) {}

/**
 * Returns the first point of the boundary.
 * @returns The first point of the boundary.
 */

glm::vec3 Bounds::getA() {
    return a;
}


/**
 * Returns the second point of the boundary.
 * @returns The second point of the boundary.
 */

glm::vec3 Bounds::getB() {
    return b;
}


/**
 * Sets point A of the bounds.
 * @param vecA - The first point of the bound.
 */

void Bounds::setA(glm::vec3 vecA) {
    a = vecA;
}


/**
 * Sets point B of the bounds.
 * @param vecB - The second point of the bound.
 */

void Bounds::setB(glm::vec3 vecB) {
    b = vecB;
}


/**
 * Returns if the vector `test` is contained within the Bounds.
 * @param test - The vector to test against the Bounds.
 * @returns a boolean indicating if the vectors intersect.
 */

bool Bounds::intersects(glm::vec3 test) {
    return (test.x >= a.x && test.x <= b.x
         && test.y >= a.y && test.y <= b.y
         && test.z >= a.z && test.z <= b.z);
}
