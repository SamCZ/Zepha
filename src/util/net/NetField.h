//
// Created by aurailus on 2020-08-05.
//

#pragma once

#include <memory>
#include <glm/vec3.hpp>

#include "util/CovariantPtr.h"
#include "world/dim/ent/Entity.h"
#include "game/atlas/DefinitionAtlas.h"

enum class NetField {
	// General
	ID,
	
	DIM,
	POS,
	VEL,
	ROT,
	SCALE,
	
	// Entities
	DISPLAY,
	VISUAL_OFF,
	ANIM_RANGE,
	ANIM_STATE,
	
	// Players
	LOOK_OFF,
	LOOK_PITCH,
	LOOK_YAW,
	
	FLYING,
	
	HAND_INV,
	WIELD_INV,
	WIELD_INDEX,
};
