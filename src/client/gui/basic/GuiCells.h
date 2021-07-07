#pragma once

#include "client/gui/GuiComponent.h"

class GuiCells : public GuiComponent {
public:
	GuiCells() = default;
	GuiCells(const string& key) : GuiComponent(key) {};
	
	void create(f32 scale, u16 count);
	void setCellColor(u16vec2 pos, vec4 color);
	void refresh();

private:
	u16 count;
	vec<vec4> cells;
};

