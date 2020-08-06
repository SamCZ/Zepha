//
// Created by aurailus on 29/08/19.
//

#pragma once

#include <map>
#include <vector>
#include <string>
#include <glm/vec2.hpp>

#include "AnimationSegment.h"

class Model;

class AnimationState {
public:
    AnimationState() = default;
    explicit AnimationState(Model& source);

    void update(double delta);

    void setAnimations(const std::vector<AnimationSegment>& anims);
    void defineAnimation(const std::string& name, glm::ivec2 range);

    void setAnim(const std::string& name, double interp, bool loop);
    void setAnim(glm::ivec2 range, double interp, bool loop);

    void setPlaying(bool playing);

    double getFrame();
    void setFrame(double frame);

    glm::ivec2 getBounds();

private:
    std::map<std::string, AnimationSegment> animations;

    glm::ivec2 range;

    bool loop = false;
    bool playing = false;

    double currentFrame = 0;
    double ticksPerSecond = 0;
    unsigned int duration = 0;
};
