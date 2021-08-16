#pragma once

#include <glm/vec3.hpp>

#include "SubgameUsertype.h"

#include "Dimension.h"
#include "AnimationManager.h"
#include "util/CovariantPtr.h"
#include "world/dim/ent/LocalLuaEntity.h"

namespace Api::Usertype {
	
	/**
	 * A usertype representing an Entity, which is a dynamic object not contained within block-space.
	 */
	 
	class Entity : public SubgameUsertype {
	public:
		
		/**
		 * The bind method for Entity. Defines the usertype on the specified lua context.
		 * @param state - The sandbox state the usertype will be bound in.
		 * @param lua - The Lua context to define the usertype in.
		 * @param core - The core Zepha table.
		 */
		 
		static void bind(State state, sol::state& lua, sol::table& core);
		
		/**
		 * Constructs an entity usertype pointing to a specified entity.
		 * @param entity - The Entity to point to.
		 */
		
		Entity(EntityPtr entity) : entity(entity), animation(entity.isL() ?
             std::static_pointer_cast<AnimationManager>(std::make_shared<LocalAnimationManager>(entity)) :
             std::static_pointer_cast<AnimationManager>(std::make_shared<ServerAnimationManager>(entity))) {}
		
             
		/**
		 * Gets the unique ID of the entity.
		 * If the entity is a server-side entity, it's ID will be positive,
		 * if it's a client-side entity, it will be negative.
		 * @returns the unique id.
		 */
		
		long long get_id();
		
		/**
		 * Returns the animation manager for the entity.
		 * @returns the animation manager.
		 */
		
		sol::object get_animation_manager(sol::this_state s);
		
		
		/**
		 * Gets the entity's current position, which may be different from
		 * its visual position if it is currently interpolating between two values.
		 * @returns the current position of the entity.
		 */
		
		glm::vec3 get_pos();
		
		/**
		 * Moves the entity to the specified position,
		 * interpolating the movement over a couple of frames.
		 * @param pos - The desired position position as a vector.
		 */
		
		void set_pos(glm::vec3 pos);
		
		/**
		 * Moves the entity to the specified position without any interpolation.
		 * @param pos - The position to move the entity to.
		 */
		
		void snap_pos(glm::vec3 pos);
		
		/**
		 * Gets the entity's current velocity.
		 * @returns the current velocity of the entity.
		 */
		
		glm::vec3 get_vel();
		
		/**
		 * Sets the entity's velocity to the specified value.
		 * @param vel - The desired velocity.
		 */
		 
		void set_vel(glm::vec3 vel);
		
		/**
		 * Gets the entity's true pitch rotation, which may be different from
		 * its visual pitch if it is currently interpolating between two values.
		 * @returns the current pitch of the entity.
		 */
		
		float get_pitch();
		
		/**
		 * Sets the entity's pitch rotation,
		 * interpolating the movement over a couple of frames.
		 * @param pitch - The desired pitch - in radians.
		 */
		
		void set_pitch(float pitch);
		
		/**
		 * Sets the entity's pitch rotation, without any interpolation.
		 * @param pitch - The desired pitch, in radians.
		 */
		
		void snap_pitch(float pitch);
		
		/**
		 * Gets the entity's true yaw rotation, which may be different from
		 * its visual yaw if it is currently interpolating between two values.
		 * @returns the current yaw of the entity.
		 */
		
		float get_yaw();
		
		/**
		 * Sets the entity's yaw rotation,
		 * interpolating the movement over a couple of frames.
		 * @param yaw - The desired yaw - in radians.
		 */
		
		void set_yaw(float yaw);
		
		/**
		 * Sets the entity's yaw rotation, without any interpolation.
		 * @param yaw - The desired yaw, in radians.
		 */
		
		void snap_yaw(float yaw);
		
		/**
		 * Gets the entity's true roll rotation, which may be different from
		 * its visual roll if it is currently interpolating between two values.
		 * @returns the current roll of the entity.
		 */
		
		float get_roll();
		
		/**
		 * Sets the entity's roll rotation,
		 * interpolating the movement over a couple of frames.
		 * @param roll - The desired roll - in radians.
		 */
		
		void set_roll(float roll);
		
		/**
		 * Sets the entity's roll rotation, without any interpolation.
		 * @param roll - The desired roll, in radians.
		 */
		
		void snap_roll(float roll);
		
		/**
		 * Gets the entity's true model scale, which may be different from
		 * its visual model scale if it is currently interpolating between two values.
		 * @returns the scale multiplier of the entity's model.
		 */
		
		glm::vec3 get_scale();
		
		/**
		 * Sets the scale of the entity's model,
		 * interpolating the movement over a couple of frames.
		 * @param scale - The desired scale multiplier.
		 */
		
		void set_scale(sol::object scale);
		
		/**
		 * Sets the scale of the entity's model, without any interpolation.
		 * @param scale - The desired scale multiplier.
		 */
		
		void snap_scale(sol::object scale);
		
		/**
		 * Gets the entity's true visual-offset, which may be different from
		 * its visual visual-offset if it is currently interpolating between two values.
		 * @returns the current visual offset  of the entity.
		 */
		
		glm::vec3 get_visual_offset();
		
		/**
		 * Sets the entity's model's display offset relative to its position,
		 * interpolating the movement over a couple of frames.
		 * @param vs - The desired visual offset for the entity.
		 */
		
		void set_visual_offset(glm::vec3 vs);
		
		/**
		 * Sets the entity's model's display offset relative to its position, without any interpolation.
		 * @param vs - The desired visual offset for the entity.
		 */
		
		void snap_visual_offset(glm::vec3 vs);
		
		
		/**
		 * Returns the current dimension of the entity.
		 * @returns the entity's dimension.
		 */
		
		Dimension get_dimension();
		
		/**
		 * Moves the entity to the dimension specified.
		 * @param identifier - The identifier of the target dimension.
		 */
		
		void set_dimension(const std::string& identifier);
		
		
		/**
		 * Sets the display type of the entity, which determines how it should render.
		 * The parameters supplied should match one of the following patterns:
		 *
		 * - gameobject: Displays as a block or item.
		 *   argA: The identifier of the block / item to display.
		 * - model: Displays a loaded model.
		 *   argA: The identifier of the model to display.
		 *   argB: The texture to apply to the model.
		 *
		 * @param mode - The display type of the entity, 'gameobject' or 'model'.
		 * @param argA - A context specific argument whose purpose is determined by the display type.
		 * @param argB - A context specific argument whose purpose is determined by the display type.
		 */
		
		void set_display_type(const std::string& type, const std::string& arg, sol::optional<std::string> arg2);
		
		/**
		 * Gets the entity's current collision box.
		 * @returns the entity's bounds table, or nil if it has no collision.
		 */
		
		sol::object get_collision_box();
		
		/**
		 * Sets the entity's collision box to the passed in bounds table,
		 * or disables collision entirely if given nil.
		 * @param box - The desired collision box, or nil.
		 */
		
		void set_collision_box(sol::object box);
		
		/**
		 * Gets whether or not the entity collides with the world.
		 * @returns true if the entity collides with the world, false otherwise.
		 */
		
		bool get_collides();
		
		/**
		 * Sets whether or not the entity collides with the world.
		 * @param collides - Whether or not the entity should collide with the world.
		 */
		 
		void set_collides(bool collides);
		
		/**
		 * Returns the entity's coefficient of gravity.
		 * @returns the coefficient of gravity.
		 */
		
		float get_gravity();
		
		/**
		 * Sets the coefficient of gravity for the entity.
		 * A value of 0 will disable gravitational acceleration.
		 * @param gravity - The desired coefficient of gravity.
		 */
		 
		void set_gravity(float gravity);
		
		/** A reference to the internal entity this instance represents. */
		EntityPtr entity;
		/** The animation manager, which contains methods to alter the entity's animations. */
		std::shared_ptr<AnimationManager> animation;
	};
}