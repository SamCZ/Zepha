//
// Created by aurailus on 29/08/19.
//

#include "AnimationState.h"

#include "AnimationSegment.h"
#include "client/graph/Model.h"
#include "client/graph/ModelAnimation.h"

AnimationState::AnimationState(Model* source) {
	if (!source) return;
	const ModelAnimation& animation = source->getAnimation();
	ticksPerSecond = animation.ticksPerSecond;
	duration = animation.duration;
	range = { 0, duration };
}

void AnimationState::setAnimations(const vec<AnimationSegment>& anims) {
	for (auto& anim : anims) defineAnimation(anim.name, anim.range);
}

void AnimationState::defineAnimation(const string& name, uvec2 range) {
	animations.emplace(name, AnimationSegment{ name, range });
}

void AnimationState::update(f64 delta) {
	if (playing) {
		float frame = currentFrame + (delta * ticksPerSecond);
		if (loop) frame = fmod(frame - range.x, range.y - range.x) + range.x;
		else frame = fmin(frame, range.y);
		
		if (frame == range.y) playing = false;
		
		currentFrame = frame;
	}
}

bool AnimationState::isLooping() {
	return loop;
}

void AnimationState::setAnim(const string& name, f64 interp, bool loop) {
	auto& anim = animations[name];
	setAnim(anim.range, interp, loop);
}

void AnimationState::setAnim(uvec2 range, f64 interp, bool loop) {
	this->range = range;
	this->loop = loop;
	currentFrame = range.x;
}

bool AnimationState::isPlaying() {
	return playing;
}

void AnimationState::setPlaying(bool playing) {
	this->playing = playing;
}

void AnimationState::setFrame(f64 frame) {
	currentFrame = frame;
}

double AnimationState::getFrame() {
	return currentFrame;
}

uvec2 AnimationState::getBounds() {
	return range;
}
