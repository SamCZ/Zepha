//
// Created by aurailus on 02/03/19.
//

#include <cmath>

#include "Frustum.h"

void Frustum::setCamInternals(float fov, float ratio, float nearD, float farD) {
	this->nearD = nearD;
	this->farD = farD;
	
	nearH = std::tan(fov) * nearD;
	nearW = nearH * ratio;
	
	farH = std::tan(fov) * farD;
	farW = farH * ratio;
}

void Frustum::update(glm::vec3& pos, glm::vec3& look, glm::vec3& up, glm::vec3& right) {
	glm::vec3 fc = pos + (look * farD);
	
	ftl = fc + (up * (farH / 2.0f)) - (right * (farW / 2.0f));
	ftr = fc + (up * (farH / 2.0f)) + (right * (farW / 2.0f));
	fbl = fc - (up * (farH / 2.0f)) - (right * (farW / 2.0f));
	fbr = fc - (up * (farH / 2.0f)) + (right * (farW / 2.0f));
	
	glm::vec3 nc = pos + (look * nearD);
	
	ntl = nc + (up * (nearH / 2.0f)) - (right * (nearW / 2.0f));
	ntr = nc + (up * (nearH / 2.0f)) + (right * (nearW / 2.0f));
	nbl = nc - (up * (nearH / 2.0f)) - (right * (nearW / 2.0f));
	nbr = nc - (up * (nearH / 2.0f)) + (right * (nearW / 2.0f));
	
	planes[TOP].setPoints(ntr, ntl, ftl);
	planes[BOTTOM].setPoints(nbl, nbr, fbr);
	planes[LEFT].setPoints(ntl, nbl, fbl);
	planes[RIGHT].setPoints(nbr, ntr, fbr);
	planes[FNEAR].setPoints(ntl, ntr, nbr);
	planes[FFAR].setPoints(ftr, ftl, fbl);
}

int Frustum::pointInFrustum(glm::vec3& p) {
	for (FrustumPlane& plane : planes) {
		if (plane.distance(p) < 0) return OUTSIDE;
	}
	return INSIDE;
}

int Frustum::boxInFrustum(FrustumAABB& b) {
	int result = INSIDE;
	
	for (auto& plane : planes) {
		glm::vec3 vertexP = b.getVertexP(plane.normal);
		if (plane.distance(vertexP) < 0) return OUTSIDE;
		
		glm::vec3 vertexN = b.getVertexN(plane.normal);
		if (plane.distance(vertexN) < 0) result = INTERSECT;
	}
	return result;
}
