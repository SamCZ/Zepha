//
// Created by aurailus on 29/08/19.
//

#include "AnimationState.h"

AnimationState::AnimationState(Model &source) {
    const ModelAnimation& animation = source.getAnimation();

    currentFrame = 100;
    ticksPerSecond = animation.ticksPerSecond;
    duration = animation.duration;
    endFrame = duration;
}

void AnimationState::setAnimations(const std::vector<AnimationSegment> &anims) {
    for (auto& anim : anims) {
        defineAnimation(anim.animationName, anim.startFrame, anim.endFrame);
    }
}

void AnimationState::defineAnimation(const std::string& animationName, unsigned int startFrame, unsigned int endFrame) {
    animations.emplace(animationName, AnimationSegment {animationName, startFrame, endFrame});
}

void AnimationState::update(double delta) {
    if (playing) {
        float frame = currentFrame + (delta * ticksPerSecond);
        if (loop) frame = fmod(frame - startFrame, endFrame - startFrame) + startFrame;
        else frame = fmin(frame, endFrame);

        if (frame == endFrame) playing = false;

        currentFrame = frame;
    }
}

void AnimationState::setAnimNamed(const std::string &animationName, double interpolateTime, bool loop) {
    auto& anim = animations[animationName];
    setAnimRange(anim.startFrame, anim.endFrame, 0, loop);
}

void AnimationState::setAnimRange(unsigned int startFrame, unsigned int endFrame, double interpolateTime, bool loop) {
    //TODO: Interpolate

    this->startFrame = startFrame;
    this->endFrame = endFrame;

    currentFrame = startFrame;

    this->loop = loop;
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

std::tuple<unsigned int, unsigned int> AnimationState::getBounds() {
    return std::make_tuple(startFrame, endFrame);
}
