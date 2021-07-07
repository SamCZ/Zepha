#include <iostream>

#include "GuiMeter.h"

#include "client/graph/Model.h"

void GuiMeter::create(vec2 scale, vec4 padding, f32 smoothness) {
	this->scale = scale;
	this->padding = padding;
	this->smoothness = smoothness;
	
	setScale({ scale.x + padding.w + padding.y, scale.y + padding.x + padding.z });
	entity.setModel(make_shared<Model>());
}

void GuiMeter::updateValues(const vec<usize>& newValues) {
	if (values.size() == 0) values = newValues;
	else {
		for (usize i = 0; i < newValues.size(); i++)
			values[i] = values[i] * (smoothness) + newValues[i] * (1 - smoothness);
	}
	
	updateMesh();
}

void GuiMeter::setBudget(usize budget) {
	this->budget = budget;
	updateMesh();
}

void GuiMeter::updateMesh() {
	vec<EntityVertex> vertices;
	vertices.reserve((values.size() + budget ? 1 : 0) * 4);
	vec<u32> indices;
	indices.reserve((values.size() + budget ? 1 : 0) * 6);
	
	usize max = 0;
	for (usize i = 0; i < values.size(); i++) max += values[i];
	
	f64 start = 0;
	for (usize i = 0; i < values.size(); i++) {
		vec4 color = vec4(COLORS[i % COLORS.size()], 1);
		f64 end = i < values.size() ? start + values[i] / static_cast<f64>(max) : 1;
		
		vertices.push_back({{ start, 0,  0 }, color, { 1, 1, 1 }, false, {}, {}, {}});
		vertices.push_back({{ start, -1, 0 }, color, { 1, 1, 1 }, false, {}, {}, {}});
		vertices.push_back({{ end,   -1, 0 }, color, { 1, 1, 1 }, false, {}, {}, {}});
		vertices.push_back({{ end,   0,  0 }, color, { 1, 1, 1 }, false, {}, {}, {}});
		
		indices.push_back(0 + i * 4);
		indices.push_back(1 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(2 + i * 4);
		indices.push_back(3 + i * 4);
		indices.push_back(0 + i * 4);
		
		start = end;
	}
	
	if (budget != 0) {
		f64 width = (std::min)((static_cast<f64>(max) - budget) / static_cast<f64>(budget), 1.0);
		if (width > 0) {
			vertices.push_back({{ 0, 0,  0 }, { 1, 0, 0, 1 }, { 1, 1, 1 }, false, {}, {}, {}});
			vertices.push_back({{ 0, 0.25, 0 }, { 1, 0, 0, 1 }, { 1, 1, 1 }, false, {}, {}, {}});
			vertices.push_back({{ width, 0.25, 0 }, { 1, 0, 0, 1 }, { 1, 1, 1 }, false, {}, {}, {}});
			vertices.push_back({{ width, 0,  0 }, { 1, 0, 0, 1 }, { 1, 1, 1 }, false, {}, {}, {}});
			
			indices.push_back(0 + values.size() * 4);
			indices.push_back(1 + values.size() * 4);
			indices.push_back(2 + values.size() * 4);
			indices.push_back(2 + values.size() * 4);
			indices.push_back(3 + values.size() * 4);
			indices.push_back(0 + values.size() * 4);
		}
	}
	
	auto m = make_unique<EntityMesh>();
	m->create(vertices, indices);
	
	auto model = make_shared<Model>();
	model->fromMesh(std::move(m));
	
	entity.setModel(model);
}

const vec<vec3> GuiMeter::COLORS = {
	{ 63 / 255.f, 224 / 255.f, 208 / 255.f },
	{ 51 / 255.f, 187 / 255.f, 232 / 255.f },
	{ 94 / 255.f, 105 / 255.f, 219 / 255.f },
	{ 159 / 255.f, 94 / 255.f, 255 / 255.f },
	{ 255 / 255.f, 138 / 255.f, 243 / 255.f },
	{ 255 / 255.f, 102 / 255.f, 102 / 255.f },
	{ 255 / 255.f, 157 / 255.f, 77 / 255.f },
	{ 169 / 255.f, 212 / 255.f, 89 / 255.f },
	{ 42 / 255.f, 212 / 255.f, 119 / 255.f },
	{ 0.3, 0.3, 0.3 }
};