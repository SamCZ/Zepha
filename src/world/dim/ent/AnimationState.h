//
// Created by aurailus on 29/08/19.
//

#pragma once

#include <map>

#include "util/Types.h"
#include "AnimationSegment.h"

class Model;

class AnimationState {
	public:
	AnimationState() = default;
	
	explicit AnimationState(Model& source);
	
	void update(double delta);
	
	void setAnimations(const vec<AnimationSegment>& anims);
	
	void defineAnimation(const string& name, uvec2 range);
	
	bool isLooping();
	
	void setAnim(const string& name, f64 interp, bool loop);
	
	void setAnim(uvec2 range, f64 interp, bool loop);
	
	bool isPlaying();
	
	void setPlaying(bool playing);
	
	f64 getFrame();
	
	void setFrame(f64 frame);
	
	uvec2 getBounds();
	
private:
	std::map<string, AnimationSegment> animations;
	
	uvec2 range;
	
	bool loop = false;
	bool playing = false;
	
	f64 currentFrame = 0;
	f64 ticksPerSecond = 0;
	u32 duration = 0;
};
