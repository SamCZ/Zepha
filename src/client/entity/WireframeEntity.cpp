#include "WireframeEntity.h"

void WireframeEntity::updateMesh(const std::vector<SelectionBox>& boxes, float width) {
	this->model->fromMesh(WireframeEntity::createMesh(boxes, width));
}

uptr<EntityMesh> WireframeEntity::createMesh(const vec<SelectionBox>& boxes, f32 wireWidth, vec3 stroke, vec4 fill) {
	vec<u32> indices {};
	indices.reserve(boxes.size() * 36 * (12 + fill.a ? 1 : 0));
	vec<EntityVertex> vertices {};
	vertices.reserve(boxes.size() * 8 * 12 + (fill.a ? 24 : 0));
	
	for (const let& box : boxes) WireframeEntity::createSelectionBoxVertices(
		box, wireWidth, vec4(stroke, 1), fill, vertices, indices);
	
	uptr<EntityMesh> mesh = make_unique<EntityMesh>();
	mesh->create(vertices, indices);
	return mesh;
}

uptr<EntityMesh> WireframeEntity::createMesh(const SelectionBox& box, f32 wireWidth, vec3 stroke, vec4 fill) {
	return WireframeEntity::createMesh(vec<SelectionBox> { box }, wireWidth, stroke, fill);
}

void WireframeEntity::createSelectionBoxVertices(const SelectionBox& box, f32 wireWidth,
	vec4 stroke, vec4 fill, vec<EntityVertex>& vertices, vec<u32>& indices) {
	
	let& a = box.a;
	let& b = box.b;
	
	createStrokeVertices({ a.x, a.y, a.z }, { b.x, a.y, a.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ a.x, a.y, b.z }, { b.x, a.y, b.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ a.x, a.y, a.z }, { a.x, a.y, b.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ b.x, a.y, a.z }, { b.x, a.y, b.z }, wireWidth, stroke, vertices, indices);
	
	createStrokeVertices({ a.x, b.y, a.z }, { b.x, b.y, a.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ a.x, b.y, b.z }, { b.x, b.y, b.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ a.x, b.y, a.z }, { a.x, b.y, b.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ b.x, b.y, a.z }, { b.x, b.y, b.z }, wireWidth, stroke, vertices, indices);
	
	createStrokeVertices({ a.x, a.y, a.z }, { a.x, b.y, a.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ b.x, a.y, a.z }, { b.x, b.y, a.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ b.x, a.y, b.z }, { b.x, b.y, b.z }, wireWidth, stroke, vertices, indices);
	createStrokeVertices({ a.x, a.y, b.z }, { a.x, b.y, b.z }, wireWidth, stroke, vertices, indices);
	
	if (fill.a) {
		vec3 nml = { 0, 1, 0 };
		f32 off = wireWidth / 2;
		usize indOffset = vertices.size();
		
		vertices.push_back({{ a.x, a.y, a.z - off }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ a.x, b.y, a.z - off }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x, b.y, a.z - off }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x, a.y, a.z - off }, fill, vec3(1), false, nml, {}, {}});
		
		vertices.push_back({{ a.x, a.y, b.z + off }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ a.x, b.y, b.z + off }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x, b.y, b.z + off }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x, a.y, b.z + off }, fill, vec3(1), false, nml, {}, {}});
		
		vertices.push_back({{ a.x - off, a.y, a.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ a.x - off, b.y, a.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ a.x - off, b.y, b.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ a.x - off, a.y, b.z }, fill, vec3(1), false, nml, {}, {}});
		
		vertices.push_back({{ b.x + off, a.y, a.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x + off, b.y, a.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x + off, b.y, b.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x + off, a.y, b.z }, fill, vec3(1), false, nml, {}, {}});
		
		vertices.push_back({{ a.x, a.x - off, a.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x, a.x - off, a.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x, a.x - off, b.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ a.x, a.x - off, b.z }, fill, vec3(1), false, nml, {}, {}});
		
		vertices.push_back({{ a.x, b.y + off, a.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x, b.y + off, a.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ b.x, b.y + off, b.z }, fill, vec3(1), false, nml, {}, {}});
		vertices.push_back({{ a.x, b.y + off, b.z }, fill, vec3(1), false, nml, {}, {}});
		
		const static array<u32, 36> boxIndices {
			0, 1, 2, 2, 3, 0,
			4, 7, 6, 6, 5, 4,
			8, 11, 10, 10, 9, 8,
			12, 13, 14, 14, 15, 12,
			16, 17, 18, 18, 19, 16,
			20, 23, 22, 22, 21, 20
		};
		
		for (u32 i : boxIndices) indices.push_back(i + indOffset);
	}
}

void WireframeEntity::createStrokeVertices(vec3 a, vec3 b, f32 wireWidth,
	vec4 color, vec<EntityVertex>& vertices, vec<u32>& indices) {
	
	f32 off = wireWidth / 2.f;
	vec3 nml = { 0, 1, 0 };
	usize indOffset = vertices.size();
	
	vertices.push_back({{ a.x - off, a.y - off, a.z - off }, color, vec3(1), false, nml, {}, {}});
	vertices.push_back({{ b.x + off, a.y - off, a.z - off }, color, vec3(1), false, nml, {}, {}});
	vertices.push_back({{ b.x + off, a.y - off, b.z + off }, color, vec3(1), false, nml, {}, {}});
	vertices.push_back({{ a.x - off, a.y - off, b.z + off }, color, vec3(1), false, nml, {}, {}});
	
	vertices.push_back({{ a.x - off, b.y + off, a.z - off }, color, vec3(1), false, nml, {}, {}});
	vertices.push_back({{ b.x + off, b.y + off, a.z - off }, color, vec3(1), false, nml, {}, {}});
	vertices.push_back({{ b.x + off, b.y + off, b.z + off }, color, vec3(1), false, nml, {}, {}});
	vertices.push_back({{ a.x - off, b.y + off, b.z + off }, color, vec3(1), false, nml, {}, {}});

	const static array<u32, 36> boxIndices {
		0, 1, 2, 2, 3, 0,
		4, 7, 6, 6, 5, 4,
		0, 4, 5, 5, 1, 0,
		3, 2, 6, 6, 7, 3,
		0, 3, 7, 7, 4, 0,
		1, 5, 6, 6, 2, 1,
	};
	
	for (u32 i : boxIndices) indices.push_back(i + indOffset);
}