//
// Created by aurailus on 25/09/19.
//

#pragma once

#include <vector>
#include <noise/noise.h>

#include "Shader.h"

#include "../Texture.h"

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

    Uniforms uniforms {};

    Texture swayTex;
    double swayOffset = 0;
    noise::module::Perlin swayNoise;
    std::vector<unsigned char> swayData {};

    glm::ivec2 windowSize {};
    float bufferScale = 1;
};
