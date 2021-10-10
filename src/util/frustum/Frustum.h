#pragma once

#include <glm/vec3.hpp>

#include "util/Types.h"
#include "FrustumAABB.h"
#include "FrustumPlane.h"

/** Checks if boxes or points are within the camera's frustum cone. */
class Frustum {
private:
	enum class Direction : u8 {
		TOP = 0,
		BOTTOM,
		LEFT,
		RIGHT,
		Near,
		FAR
	};
public:
	enum class Intersection { OUTSIDE, INSIDE, INTERSECTS };
	
	void setCamInternals(f32 angle, f32 ratio, f32 nearDistance, f32 farDistance);
	
	void update(vec3& pos, vec3& look, vec3& up, vec3& right);
	
	Intersection pointInFrustum(vec3& p);
	
	Intersection boxInFrustum(FrustumAABB& b);
	
	FrustumPlane planes[6];
	
	f32 nearDistance, farDistance;
	f32 nearWidth, nearHeight, farWidth, farHeight;
	
	vec3 ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;
};

