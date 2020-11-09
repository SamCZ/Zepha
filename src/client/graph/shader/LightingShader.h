//
// Created by aurailus on 24/09/19.
//

#pragma once

#include <glm/vec2.hpp>

#include "Shader.h"

class LightingShader : public Shader {
	public:
	explicit LightingShader(glm::ivec2 windowSize, float bufferScale);
	
	void postCreate() override;
	
	void windowResized(glm::ivec2 windowSize);
	
	struct Uniforms {
		GLint gPosition;
		GLint gNormal;
		GLint gColorSpec;
		GLint ssaoSampler;
		
		GLint camPosition;
	};
	
	Uniforms uniforms{};
	
	unsigned int gBuffer = 0;
	unsigned int gPosition = 0;
	unsigned int gNormal = 0;
	unsigned int gColorSpec = 0;
	unsigned int rDepth = 0;
	
	private:
	glm::ivec2 windowSize{};
	float bufferScale = 1;
};
