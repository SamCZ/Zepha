//
// Created by aurailus on 02/03/19.
//

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtx/normal.hpp>

#include "FrustumPlane.h"

void FrustumPlane::setPoints(vec3& v1, vec3& v2, vec3& v3) {
	normal = glm::triangleNormal(v1, v2, v3);
	point = v2;
	d = -glm::dot(normal, point);
}

float FrustumPlane::distance(vec3& p) {
	return (d + glm::dot(normal, p));
}