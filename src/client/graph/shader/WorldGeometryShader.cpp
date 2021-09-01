//
// Created by aurailus on 25/09/19.
//

#include <glm/vec2.hpp>

#include "WorldGeometryShader.h"

WorldGeometryShader::WorldGeometryShader(glm::ivec2 windowSize, float bufferScale) : Shader(),
	windowSize(windowSize),
	bufferScale(bufferScale),
	swayData(16 * 4 * 16) {
	
	swayNoise = FastNoise::New<FastNoise::Constant>();
}

void WorldGeometryShader::postCreate() {
	swayTex.loadFromBytes(&swayData[0], 16, 16, GL_LINEAR, GL_MIRRORED_REPEAT);
	
	uniforms.proj = get("projection");
	uniforms.model = get("model");
	uniforms.view = get("view");
	
	uniforms.swaySampler = get("swayTex");
	uniforms.time = get("time");
	
	use();
	set(uniforms.swaySampler, 1);
}

void WorldGeometryShader::windowResized(glm::ivec2 windowSize) {
	this->windowSize = windowSize;
}

void WorldGeometryShader::updateSwayMap(double delta) {
	swayOffset += delta * 2.8;
	for (int i = 0; i < 16 * 16; i++) {
		swayData[i * 4] = static_cast<unsigned char>(
			(fmax(-1, fmin(1, swayNoise->GenSingle3D((i / 16) / 3.f, (i % 16) / 3.f, swayOffset, 0))) + 1) / 2.f * 255.f);
		swayData[i * 4 + 1] = static_cast<unsigned char>(
			(fmax(-1, fmin(1, swayNoise->GenSingle3D((i / 16) / 3.f, (i % 16) / 3.f, swayOffset + 50, 0))) + 1) / 2.f * 255.f);
		swayData[i * 4 + 2] = static_cast<unsigned char>(
			(fmax(-1, fmin(1, swayNoise->GenSingle3D((i / 16) / 3.f, (i % 16) / 3.f, swayOffset + 100, 0))) + 1) / 2.f *
			255.f);
	}
	swayTex.updateTexture(0, 0, 16, 16, &swayData[0]);
}