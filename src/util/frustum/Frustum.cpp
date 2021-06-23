//
// Created by aurailus on 02/03/19.
//

#include <cmath>

#include "Frustum.h"

void Frustum::setCamInternals(f32 fov, f32 ratio, f32 nearDistance, f32 farDistance) {
	this->nearDistance = nearDistance;
	this->farDistance = farDistance;
	
	nearHeight = std::tan(fov) * nearDistance;
	nearWidth = nearHeight * ratio;
	
	farHeight = std::tan(fov) * farDistance;
	farWidth = farHeight * ratio;
}

void Frustum::update(vec3& pos, vec3& look, vec3& up, vec3& right) {
	glm::vec3 fc = pos + (look * farDistance);
	
	ftl = fc + (up * (farHeight / 2.f)) - (right * (farWidth / 2.f));
	ftr = fc + (up * (farHeight / 2.f)) + (right * (farWidth / 2.f));
	fbl = fc - (up * (farHeight / 2.f)) - (right * (farWidth / 2.f));
	fbr = fc - (up * (farHeight / 2.f)) + (right * (farWidth / 2.f));
	
	glm::vec3 nc = pos + (look * nearDistance);
	
	ntl = nc + (up * (nearHeight / 2.f)) - (right * (nearWidth / 2.f));
	ntr = nc + (up * (nearHeight / 2.f)) + (right * (nearWidth / 2.f));
	nbl = nc - (up * (nearHeight / 2.f)) - (right * (nearWidth / 2.f));
	nbr = nc - (up * (nearHeight / 2.f)) + (right * (nearWidth / 2.f));
	
	planes[static_cast<u8>(Direction::TOP   )].setPoints(ntr, ntl, ftl);
	planes[static_cast<u8>(Direction::BOTTOM)].setPoints(nbl, nbr, fbr);
	planes[static_cast<u8>(Direction::LEFT  )].setPoints(ntl, nbl, fbl);
	planes[static_cast<u8>(Direction::RIGHT )].setPoints(nbr, ntr, fbr);
	planes[static_cast<u8>(Direction::NEAR  )].setPoints(ntl, ntr, nbr);
	planes[static_cast<u8>(Direction::FAR   )].setPoints(ftr, ftl, fbl);
}

Frustum::Intersection Frustum::pointInFrustum(vec3& p) {
	for (FrustumPlane& plane : planes) {
		if (plane.distance(p) < 0) return Intersection::OUTSIDE;
	}
	return Intersection::INSIDE;
}

Frustum::Intersection Frustum::boxInFrustum(FrustumAABB& b) {
	Intersection res = Intersection::INSIDE;
	
	for (auto& plane : planes) {
		vec3 vertexP = b.getVertexP(plane.normal);
		if (plane.distance(vertexP) < 0) return Intersection::OUTSIDE;
		
		vec3 vertexN = b.getVertexN(plane.normal);
		if (plane.distance(vertexN) < 0) res = Intersection::INTERSECTS;
	}
	
	return res;
}
