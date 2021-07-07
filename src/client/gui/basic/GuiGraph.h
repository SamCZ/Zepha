//
// Created by aurailus on 08/02/19.
//

#pragma once

#include "client/gui/GuiComponent.h"

class AtlasRef;

class GuiGraph : public GuiComponent {
public:
	GuiGraph() = default;
	
	GuiGraph(const std::string& key);
	
	void
	create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, unsigned int length, float maxValue,
		bool editInPlace);
	
	void setMax(float max);
	
	void pushValue(float value);
	
private:
	void buildHistogramMesh();
	
	bool editInPlace = false;
	
	unsigned int insertionPoint = 0;
	unsigned int length = 60;
	
	bool dynamicMax = true;
	float maxVal = 0;
	
	std::shared_ptr<AtlasRef> texture;
	std::vector<float> history;
};

