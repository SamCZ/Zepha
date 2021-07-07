#pragma once
#include "client/gui/GuiComponent.h"

class GuiMeter : public GuiComponent {
public:
	GuiMeter() = default;
	GuiMeter(const string& key) : GuiComponent(key) {};
	
	void create(vec2 scale, vec4 padding, f32 smoothness = 0);
	void setBudget(usize budget);
	void updateValues(const vec<usize>& values);

	const static vec<vec3> COLORS;
	
private:
	void updateMesh();
	
	usize budget;
	f32 smoothness;
	vec<usize> values;
};
