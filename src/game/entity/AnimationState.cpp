//
// Created by aurailus on 29/08/19.
//

#include "AnimationState.h"

AnimationState::AnimationState(Model &source) {
    const ModelAnimation& animation = source.getAnimation();

    currentFrame = 100;
    ticksPerSecond = animation.ticksPerSecond;
    duration = animation.duration;
}

void AnimationState::defineAnimation(const std::string& animationName, uint startFrame, uint endFrame) {
    animations.emplace(animationName, AnimationSegment {animationName, startFrame, endFrame});
}

void AnimationState::update(double delta) {
    uint start = (currentAnimation ? currentAnimation->startFrame : 0);
    uint end = (currentAnimation ? currentAnimation->endFrame : duration);

    //TODO: Implement loop toggle

    if (playing) currentFrame = fmod((currentFrame - start) + (delta * ticksPerSecond), end - start) + start;
}

void AnimationState::setAnimation(const std::string &animationName, double interpolateTime, bool loop) {
    //TODO: Interpolate
    currentAnimation = &animations[animationName];
    currentFrame = currentAnimation->startFrame;
    looping = loop;
}

void AnimationState::setPlaying(bool playing) {
    this->playing = playing;
}

void AnimationState::setFrame(double frame) {
    currentFrame = frame;
}

double AnimationState::getFrame() {
    return currentFrame;
}

std::tuple<int, int> AnimationState::getBounds() {
    if (currentAnimation) return std::make_tuple(
        static_cast<int>(currentAnimation->startFrame),
        static_cast<int>(currentAnimation->endFrame)
    );
    else return std::make_tuple<int>(0, duration);

}
