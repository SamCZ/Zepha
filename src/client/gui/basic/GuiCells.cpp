//#include <iostream>
//#include "GuiCells.h"
//
//void GuiCells::create(f32 scale, u16 count) {
//	this->scale = vec4(scale);
//	this->count = count;
//	cells = vec<vec4>(count * count);
//}
//
//void GuiCells::setCellColor(u16vec2 pos, vec4 color) {
//	cells[pos.x + pos.y * count] = color;
//}
//
//void GuiCells::refresh() {
//	vec<EntityVertex> vertices;
//	vertices.reserve(count * count * 4);
//	vec<u32> indices;
//	indices.reserve(count * count * 6);
//
//	for (u16 i = 0; i < count; i++) {
//		for (u16 j = 0; j < count; j++) {
//			vertices.push_back({{ i, j, 0 }, cells[i + j * count], { 1, 1, 1 }, false, {}, {}, {}});
//			vertices.push_back({{ i, j + 1, 0 }, cells[i + j * count], { 1, 1, 1 }, false, {}, {}, {}});
//			vertices.push_back({{ i + 1, j + 1, 0 }, cells[i + j * count], { 1, 1, 1 }, false, {}, {}, {}});
//			vertices.push_back({{ i + 1, j,  0 }, cells[i + j * count], { 1, 1, 1 }, false, {}, {}, {}});
//
//			indices.push_back(0 + (i + j * count) * 4);
//			indices.push_back(1 + (i + j * count) * 4);
//			indices.push_back(2 + (i + j * count) * 4);
//			indices.push_back(2 + (i + j * count) * 4);
//			indices.push_back(3 + (i + j * count) * 4);
//			indices.push_back(0 + (i + j * count) * 4);
//		}
//	}
//
//	auto m = make_unique<EntityMesh>();
//	m->create(vertices, indices);
//
//	auto model = make_shared<Model>();
//	model->fromMesh(std::move(m));
//
//	entity.setModel(model);
//	setScale(this->scale);
//}