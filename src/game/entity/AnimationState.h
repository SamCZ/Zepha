//
// Created by aurailus on 29/08/19.
//

#pragma once

#include "Model.h"
#include "ModelAnimation.h"
#include "AnimationSegment.h"

class AnimationState {
public:
    AnimationState() = default;
    explicit AnimationState(Model& source);

    void update(double delta);

    void setAnimations(const std::vector<AnimationSegment>& anims);
    void defineAnimation(const std::string& animationName, uint startFrame, uint endFrame);

    void setAnimNamed(const std::string& animationName, double interpolateTime, bool loop);
    void setAnimRange(unsigned int startFrame, unsigned int endFrame, double interpolateTime, bool loop);

    void setPlaying(bool playing);

    void setFrame(double frame);
    double getFrame();
    std::tuple<unsigned int, unsigned int> getBounds();

private:
    std::map<std::string, AnimationSegment> animations;

    unsigned int startFrame = 0;
    unsigned int endFrame = 0;

    bool playing = false;
    bool loop = false;

    double currentFrame = 0;
    double ticksPerSecond = 0;
    uint duration = 0;
};
