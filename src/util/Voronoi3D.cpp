#include <stb_image_write.h>

#include "Voronoi3D.h"

#include "util/Space.h"

Voronoi3D::Voronoi3D(u16 size) : size(size), data(pow(size, 3)) {}

Voronoi3D::Voronoi3D(u16 size, const vec<VoronoiPoint>& data) : size(size), data(pow(size, 3)), points(data) {
	generate();
}

void Voronoi3D::setData(const vec<VoronoiPoint>& data) {
	this->points = data;
	generate();
}

void Voronoi3D::generate() {
	for (u32 i = 0; i < pow(size, 3); i++) {
		const u16vec3 pos = Space::indexToPos(i, size);
		
		f32 pointDistance = INFINITY;
		u16 currentInd = 0;
		
		for (u32 j = 0; j < points.size(); j++) {
			f32 thisPointDistance = glm::distance(vec3(points[j].pos), vec3(pos));
			if (thisPointDistance < pointDistance) {
				pointDistance = thisPointDistance;
				currentInd = j;
			}
		}
		
		data[i] = currentInd;
	}
}

unsigned short Voronoi3D::operator[](u32 ind) {
	return points[data[ind]].data;
}
unsigned short Voronoi3D::operator[](u16vec3 pos) {
	return operator[](Space::posToIndex(pos, size));
}

void Voronoi3D::outputImage(u16 yCount) {
	vec<u8> colorData(pow(size, 2) * 3);

	for (u32 i = 0; i < yCount; i++) {
		for (u32 j = 0; j < pow(size, 2); j++) {
			u16 x = j % size;
			u16 z = j / size;
			
			vec3 color = points[data[Space::posToIndex(
				{ x, floor(static_cast<f32>(i) / (yCount - 1) * (size - 1)), z }, size)]].color;
			
			colorData[j * 3 + 0] = static_cast<u8>(color.x * 255);
			colorData[j * 3 + 1] = static_cast<u8>(color.y * 255);
			colorData[j * 3 + 2] = static_cast<u8>(color.z * 255);
		}
		
		string name = "voronoi_" + std::to_string(i) + ".jpg";
		stbi_write_jpg(name.data(), size, size, 3, colorData.data(), 100);
	}
}