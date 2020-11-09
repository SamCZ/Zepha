//
// Created by aurailus on 24/09/19.
//

#pragma once

#include <random>
#include <glm/vec2.hpp>

#include "Shader.h"

class SSAOShader : public Shader {
	public:
	explicit SSAOShader(glm::ivec2 windowSize, float bufferScale, unsigned int kernelCount);
	
	void postCreate() override;
	
	void windowResized(glm::ivec2 windowSize);
	
	struct Uniforms {
		GLint proj;
		GLint view;
		GLint kernelCount;
		GLint sampleScale;
	};
	
	Uniforms uniforms{};
	
	unsigned int kernelCount = 32;
	unsigned int sampleCount = 16;
	
	std::vector<glm::vec3> kernels{};
	std::vector<glm::vec3> noise{};
	
	unsigned int tex = 0;
	unsigned int fbo = 0;
	unsigned int colorBuffer = 0;
	
	private:
	glm::ivec2 windowSize{};
	float bufferScale = 1;
};
