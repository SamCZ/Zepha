#pragma once

#include "util/Types.h"

/** An axis-aligned bounding box used in frustum calculations. */
class FrustumAABB {
public:
	vec3 corner;
	vec3 size;
	
	FrustumAABB(vec3 a, vec3 b);
	
	void set(vec3 a, vec3 b);
	
	vec3 getVertexP(vec3& normal);
	
	vec3 getVertexN(vec3& normal);
};

