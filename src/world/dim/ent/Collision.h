#pragma once

#include <glm/vec3.hpp>

#include "util/CovariantPtr.h"

class Subgame;
class Dimension;
class SelectionBox;

class Collision {
public:
	
	/**
	 * Checks if a collision box is "on the ground", meaning it is
	 * a suitably small distance away from the nearest bottom collision.
	 * The velocity is also tested, and the ground-check will only succeed if the y velocity is negative.
	 *
	 * @param game - A reference to the subgame.
	 * @param dim - A reference to the dimension to test in.
	 * @param collisionBox - The collision box to test.
	 * @param pos - The position of the collision box.
	 * @param vel - The velocity of the collision box.
	 * @returns a boolean indicating if the collision box is on the ground.
	 */
	 
	static bool isOnGround(SubgamePtr game, DimensionPtr dim,
		const SelectionBox& collisionBox, glm::vec3 pos, glm::vec3 vel);
	
	/**
	 * Attempts to move a collision box in the world according to its
	 * velocity, not allowing movement into solid blocks.
	 * Returns the new position and velocity in a tuple.
	 *
	 * @param game - A reference to the subgame.
	 * @param dim - A reference to the dimension to act upon.
	 * @param collisionBox - The collision box to attempt to move.
	 * @param pos - The initial position of the collision box.
	 * @param vel - The velocity to apply to the collision box.
	 * @param delta - The delta time since last frame, used in velocity calculations.
	 * @returns a tuple whose first value is the new positon, and the second is the new velocity.
	 */
	 
	static std::tuple<glm::vec3, glm::vec3> applyVel(SubgamePtr game, DimensionPtr dim,
		const SelectionBox& collisionBox, glm::vec3 pos, glm::vec3 vel, float delta);
	
	/**
	 * Checks if a collision box collides at a specified position.
	 *
	 * @param game - A reference to the subgame.
	 * @param dim - A reference to the dimension to test in.
	 * @param collisionBox - The collision box to test.
	 * @param pos - The position to test the collision box at.
	 * @returns a boolean indicating if the collision box collides at the specified position.
	 */
	 
	static bool collidesAt(SubgamePtr game, DimensionPtr dim, const SelectionBox& collisionBox, glm::vec3 pos);
};
