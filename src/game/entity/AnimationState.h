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

    void defineAnimation(const std::string& animationName, uint startFrame, uint endFrame);

    void setAnimation(const std::string& animationName, double interpolateTime, bool loop);
    void setPlaying(bool playing);

    void setFrame(double frame);
    double getFrame();
    std::tuple<int, int> getBounds();

private:
    AnimationSegment* currentAnimation = nullptr;
    std::map<std::string, AnimationSegment> animations;

    bool playing = false;
    bool looping = false;

    double currentFrame = 0;
    double ticksPerSecond = 0;
    uint duration = 0;
};
