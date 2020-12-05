#pragma once

/**
 * An enum class containing all of the potential data sequences
 * that may be sent over the network for players or entities.
 * ALL is a special value that should never be sent, and instead
 * indicates to serialization functions to include all of the available data in the packet.
 */

enum class NetField {
	ALL = 99,
	ID = 0,
	
	POS = 10,
	VEL = 11,
	ROT = 12,
	SCALE = 13,
	VISUAL_OFF = 14,
	
	DISPLAY = 20,
	ANIM_RANGE = 21,
	ANIM_STATE = 22,
	
	DIM = 30,
	
	COLLISION_BOX = 40,
	COLLIDES = 41,
	GRAVITY = 42,
	
	// Player Specific
	
	LOOK_OFF = 50,
	LOOK_PITCH = 51,
	LOOK_YAW = 52,
	FLYING = 53,
	HAND_INV = 54,
	WIELD_INV = 55,
	WIELD_INDEX = 56
};
