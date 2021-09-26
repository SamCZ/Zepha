#pragma once

#include "util/Types.h"

/**
 * Creates a three-dimensional voronoi diagram which can be
 * sampled to find the nearest point to a given position.
 */

class Voronoi3D {
public:
	
	/** Represents a point of data in the voronoi map. */
	struct VoronoiPoint {
		VoronoiPoint(u16vec3 pos, u16 data): pos(pos), data(data) {}
		VoronoiPoint(u16vec3 pos, u16 data, vec3 color): pos(pos), data(data), color(color) {}
		
		u16vec3 pos;
		u16 data;
		vec3 color;
	};
	
	Voronoi3D() = default;
	
	explicit Voronoi3D(u16 size);
	
	Voronoi3D(u16 size, const vec<VoronoiPoint>& data);
	
	/** Sets the data to the points specified and generates the map. */
	void setData(const vec<VoronoiPoint>& data);
	
	/** Retrieves a point data value at the specified index. */
	u16 operator[](u32 ind);
	
	/** Retrieves a point data value at the specified position. */
	u16 operator[](u16vec3 pos);

	/** Outputs a debug image into the working directory, with the number of layers specified by yCount. */
    void outputImage(u16 yCount);
	
private:
	
	/** Generates the voronoi diagram. */
	void generate();
	
	/** The size of one axis of the voronoi map. */
	u16 size;
	
	/** A 3d grid of data corresponding to VoronoiPoint indices. */
	vec<u16> data;
	
	/** The voronoi points that the map was generated with. */
	vec<VoronoiPoint> points;
};
