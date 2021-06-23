#pragma once

#include "util/Types.h"

/** Represents a frustum plane. */
class FrustumPlane {
public:
	vec3 normal;
	vec3 point;
	f32 d;
	
	void setPoints(vec3& v1, vec3& v2, vec3& v3);
	
	f32 distance(vec3& p);
};

