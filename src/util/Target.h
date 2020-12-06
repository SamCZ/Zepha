#pragma once

#include "util/Vec.h"
#include "util/CovariantPtr.h"

/**
 * An structure representing a targeted element,
 * be it a block, entity, or nothing.
 */

class Target {
public:
	
	/** A struct containing data for a targeted entity. */
	struct EntityData { long long id; };
	/** A struct containing data for a targeted block & face. */
	struct BlockData { glm::ivec3 pos; EVec face; };
	/** A struct containing no data for targets pointing to nothing. */
	struct NothingData {};
	
	/** An enum of the potential types of things a Target can point to. */
	enum class Type { ENTITY, BLOCK, NOTHING };
	/** A union of potential data structs for the three target types. */
	union TargetData { EntityData entity; BlockData block; NothingData _; };
	
	/**
	 * Creates an empty target pointing at nothing.
	 */
	 
	Target() = default;
	
	/**
	 * Copy constructor - duplicates a target.
	 * @param o - The target to copy.
	 */
	 
	Target(const Target& o) = default;
	
	/**
	 * Creates a target pointing at an entity, with the ID and dimension specified.
	 * @param dim - The dimension of the entity.
	 * @param id - The ID of the entity.
	 */
 
	Target(DimensionPtr dim, unsigned int id);
	
	/**
	 * Creates a target pointing at a specific face of a block, in the dimension specified.
	 * @param dim - The dimension of the entity.
	 * @param pos - The block position to target.
	 * @param face - The face of the block to target.
	 */
	 
	Target(DimensionPtr dim, glm::ivec3 pos, EVec face);
	
	/**
	 * For block targets, gets the position that is one unit away from the targeted face.
	 * @throws if the target type is not a block.
	 * @returns the position above the targeted face.
	 */
	 
	glm::ivec3 getAbovePos() const;
	
	/** The current type of thing being targeted. */
	Type type = Target::Type::NOTHING;
	/** Data for the current targeted thing. */
	TargetData data = { ._ = {} };
	/** The dimension in which the current targeted thing resides. */
	DimensionPtr dim = nullptr;
};
