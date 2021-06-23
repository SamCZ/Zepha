#pragma once

#include "util/Types.h"
#include "AnimationState.h"
#include "util/CovariantPtr.h"
#include "game/def/mesh/SelectionBox.h"

/**
 * Represents a dynamic renderable object that is not contained within block-space.
 */
 
class Entity {
public:
	Entity() = default;
	
	Entity(SubgamePtr game, DimensionPtr dim) : game(game), dim(dim) {};
	
	/**
	 * Gets the unique ID of the entity.
	 * If the entity is a server-side entity, it's ID will be positive,
	 * if it's a client-side entity, it will be negative.
	 * @returns the unique id.
	 */
	 
	virtual i64 getId() const;
	
	/**
	 * Sets the unique ID of the entity.
	 * If the entity is a server-side entity, it's ID should be positive,
	 * if it's a client-side entity, it should be negative.
	 * @param newId - The desired ID.
	 */
	 
	virtual void setId(i64 newId);
	
	/**
	 * Gets the entity's current position.
	 * @returns the entity's current position as a vector.
	 */
	 
	virtual vec3 getPos() const;
	
	/**
	 * Moves the entity to the specified position.
	 * @param newPos - The desired position as a vector.
	 */
	
	virtual void setPos(vec3 newPos);
	
	/**
	 * Gets the entity's current velocity.
	 * @returns the entity's current velocity as a vector.
	 */
	
	virtual vec3 getVel() const;
	
	/**
	 * Moves the entity to the specified position.
	 * @param newVel - The desired velocity as a vector.
	 */
	
	virtual void setVel(vec3 newVel);
	
	/**
	 * Returns the entity's current rotation across all 3 axises as a vector.
	 * @returns the entity's rotation.
	 */
	
	virtual vec3 getRot() const;
	
	/**
	 * Sets the entity's rotation across all three axises.
	 * @param newRot - The desired rotation of the entity.
	 */
	
	virtual void setRot(vec3 newRot);
	
	/**
	 * Returns the X component of the entity's rotation vector.
	 * @returns the entity's X rotation.
	 */
	
	virtual f32 getRotateX() const;
	
	/**
	 * Sets the X component of the entity's rotation vector.
	 * @param newRotateX - The desired rotation, in radians.
	 */
	
	virtual void setRotateX(f32 newRotateX);
	
	/**
	 * Returns the Y component of the entity's rotation vector.
	 * @returns the entity's Y rotation.
	 */
	
	virtual f32 getRotateY() const;
	
	/**
	 * Sets the Y component of the entity's rotation vector.
	 * @param newRotateY - The desired rotation, in radians.
	 */
	
	virtual void setRotateY(f32 newRotateY);
	
	/**
	 * Returns the Z component of the entity's rotation vector.
	 * @returns the entity's Z rotation.
	 */
	
	virtual f32 getRotateZ() const;
	
	/**
	 * Sets the Z component of the entity's rotation vector.
	 * @param newRotateZ - The desired rotation, in radians.
	 */
	
	virtual void setRotateZ(f32 newRotateZ);
	
	/**
	 * Gets the entity's model scale.
	 * @returns the scale vector of the entity's model.
	 */
	 
	virtual vec3 getScale() const;
	
	/**
	 * Sets the scale of the entity's model to a float value.
	 * @param newScale - The desired scale multiplier.
	 */
	
	virtual void setScale(f32 newScale);
	
	/**
	 * Sets the scale of the entity's model to a vector.
	 * @param newScale - The desired scale multiplier.
	 */
	
	virtual void setScale(vec3 newScale);
	
	/**
	 * Gets the entity's model's display offset relative to its position.
	 * @returns the visual offset of the entity.
	 */
	
	virtual vec3 getVisualOffset() const;
	
	/**
	 * Sets the entity's model's display offset relative to its position.
	 * @param newVisualOff - The desired visual offset for the entity.
	 */
	
	virtual void setVisualOffset(vec3 newVisualOff);
	
	
	/**
	 * Gets the entity's current dimension.
	 * @returns a pointer to the entity's current dimension.
	 */
	
	virtual DimensionPtr getDim() const;
	
	/**
	 * Sets the entity's dimension pointer to the value specified.
	 * @param newDim - The dimension to assign to the entity.
	 */
	
	virtual void setDim(DimensionPtr newDim);
	
	
	/**
	 * Returns the entity's collision box, if it has one.
	 * @returns an optional that will contain the entity's collision box, if it has one.
	 */
	
	virtual std::optional<SelectionBox> getCollisionBox() const;
	
	/**
	 * Sets the entity's collision box to the box specified.
	 * @param newCollisionBox - The new collision box.
	 */
	
	virtual void setCollisionBox(const SelectionBox& newCollisionBox);
	
	/**
	 * Removes the entity's collision box.
	 */
	
	virtual void removeCollisionBox();
	
	/**
	 * Gets whether or not the entity collides with the world.
	 * @returns true if the entity collides with the world, false otherwise.
	 */
	
	virtual bool getCollides() const;
	
	/**
	 * Sets whether or not the entity collides with the world.
	 * @param newCollides - Whether or not the entity should collide with the world.
	 */
	
	virtual void setCollides(bool newCollides);
	
	/**
	 * Returns the entity's coefficient of gravity.
	 * @returns the entity's coefficient of gravity.
	 */
	
	virtual f32 getGravity() const;
	
	/**
	 * Sets the entity's coefficient of gravity.
	 * A value of 0 will disable gravity altogether.
	 * @param newGravity - The desired coefficient of gravity.
	 */
	
	virtual void setGravity(f32 newGravity);
	
	
	/**
	 * Gets the subgame pointer.
	 * @returns a reference to the current subgame
	 */
	
	SubgamePtr getGame() const;
	
	/**
	 * Updates the entity's animations, velocity, and position.
	 * @param delta - The last frame's delta time.
	 */
	
	virtual void update(f64 delta);
	
	/** The animation state of the entity. */
	AnimationState animation {};
	
protected:
	SubgamePtr game = nullptr;
	DimensionPtr dim = nullptr;
	
	/** The entity's unique id, positive if it is a server-side entity, negative if it is client-side. */
	i64 id = 0;
	
	vec3 pos {};
	vec3 vel {};
	vec3 rot {};
	vec3 visualOff {};
	vec3 scale { 1, 1, 1 };
	
	std::optional<SelectionBox> selectionBox {};
	std::optional<SelectionBox> collisionBox {};
	
	f32 gravity = 0;
	bool collides = false;
};
