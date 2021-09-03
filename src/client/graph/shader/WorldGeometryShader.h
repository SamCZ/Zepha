//
// Created by aurailus on 25/09/19.
//

#pragma once

#include <FastNoise/FastNoise.h>

#include "Shader.h"

#include "util/Types.h"
#include "client/graph/Texture.h"

class WorldGeometryShader : public Shader {
public:
	explicit WorldGeometryShader(glm::ivec2 windowSize, float bufferScale);
	
	void postCreate() override;
	
	void windowResized(glm::ivec2 windowSize);
	
	void updateSwayMap(double delta);
	
	struct Uniforms {
		GLint proj;
		GLint model;
		GLint view;
		
		GLint swaySampler;
		
		GLint time;
	};
	
	Uniforms uniforms{};
	
	Texture swayTex;
	vec<u8> swayData{};
	f64 swayOffset = 0;
	FastNoise::SmartNode<> swayGenerator;
	
	glm::ivec2 windowSize{};
	float bufferScale = 1;
};
