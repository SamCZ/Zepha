#pragma once

#include "world/dim/ent/DrawableEntity.h"

#include "game/def/mesh/SelectionBox.h"
#include "client/graph/mesh/EntityVertex.h"

class WireframeEntity : public DrawableEntity {
public:
	WireframeEntity(SubgamePtr game, DimensionPtr dim, vec3 stroke, vec4 fill = vec4(0)) :
		DrawableEntity(game, dim, std::make_shared<Model>()), Entity(game, dim), stroke(stroke), fill(fill) {};
	
	void updateMesh(const vec<SelectionBox>& boxes, f32 width);
	
	static uptr<EntityMesh> createMesh(const vec<SelectionBox>& boxes,
		f32 wireWidth, vec3 stroke = vec3(1), vec4 fill = vec4(0));
	
	static uptr<EntityMesh> createMesh(const SelectionBox& box,
		f32 wireWidth, vec3 stroke = vec3(1), vec4 fill = vec4(0));
	
private:
	static void createSelectionBoxVertices(const SelectionBox& box, f32 wireWidth,
		vec4 stroke, vec4 fill, vec<EntityVertex>& vertices, vec<u32>& indices);
	
	static void createStrokeVertices(vec3 a, vec3 b, f32 wireWidth,
		vec4 color, vec<EntityVertex>& vertices, vec<u32>& indices);
	
	vec3 stroke {};
	vec4 fill {};
};
