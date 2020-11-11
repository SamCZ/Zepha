#pragma once

#include <mutex>
#include <array>
#include <vector>
#include <glm/vec3.hpp>

#include "util/Lockable.h"

#include "util/RIE.h"
#include "util/Space.h"

class Deserializer;

/**
 * A single chunk.
 * Represents a 16^3 region of blocks in the world.
 * Implements Lockable. Must be manually locked when being used across threads.
 */

class Chunk : public Lockable {
public:
	friend class MapGen;
	
	/**
	 * An enum for indicating the state of a Chunk.
	 */
	
	enum class State {
		EMPTY, PARTIAL, GENERATED
	};
	
	/**
	 * A struct for storing Block lighting at a position.
	 * Uses a bitfield, where each light channel is allocated 5 bits.
	 * Each channel can have an intensity from 0 (off), to 31 (full).
	 */
	
	struct BlockLight {
		/** The red channel */
		unsigned char r: 5;
		/** The green channel */
		unsigned char g: 5;
		/** The blue channel */
		unsigned char b: 5,
		/** A leftover bit, declared to make the struct use an even 16. */
			: 1;
	};
	
	/**
	 * A union for accessing a short as a BlockLight struct, or vice versa.
	 */
	
	typedef union {
		short sh;
		BlockLight b;
	} blocklight_union;
	
	/**
	 * A struct for storing Sunlight at two positions.
	 * Sunlight intensity ranges from 0 (none) to 15 (full), which can fit in 4 bits,
	 * so each SunLight struct can store two positions of sunlight.
	 * Sunlight with a maximum light value of 15 will cascade downwards infinitely, without losing intensity.
	 */
	
	struct SunLight {
		/** The odd positioned light value */
		unsigned char a: 4;
		/** The even positioned light value */
		unsigned char b: 4;
	};
	
	/**
	 * A union for accessing a char as a Sunlight struct, or vice versa.
	 */
	
	typedef union {
		char ch;
		SunLight s;
	} sunlight_union;
	
	/**
	 * Initialize an empty, ungenerated chunk containing only INVALID.
	 * Used in Map Generation.
	 */
	
	Chunk() = default;
	
	/**
	 * A simple copy constructor.
	 *
	 * @param o - The chunk to copy.
	 */
	
	Chunk(const Chunk& o);
	
	/**
	 * Basic chunk pos constructor, initializes an empty, ungenerated chunk
	 * that can optionally be identified as a partial.
	 *
	 * @param pos - The position of the Chunk in its dimension.
	 * @param partial - True if the chunk is a MapGen partial.
	 */
	
	Chunk(glm::ivec3 pos, bool partial = false);
	
	/**
	 * Initializes a generated chunk with the blocks and biomes RIE arrays specified.
	 * Used in Chunk deserialization from the server.
	 *
	 * @param pos - The position of the Chunk in its dimension.
	 * @param blocks - An RIE array of block positions.
	 * @param biomes - An RIE array of biome positions.
	 */
	
	Chunk(glm::ivec3 pos, const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes);
	
	/**
	 * Get the position of the chunk.
	 *
	 * @returns the position of the chunk.
	 */
	
	inline glm::ivec3 getPos() const;
	
	/**
	 * Set the position of the chunk.
	 *
	 * @param newPos - The new position of the chunk.
	 */
	
	inline void setPos(glm::ivec3 newPos);
	
	/**
	 * Get the chunk's dirty state, which is whether it needs to be remeshed or not.
	 * This value is only set through other classes using setDirty.
	 *
	 * @returns if the chunk is dirty.
	 */
	
	inline bool isDirty() const;
	
	/**
	 * Indicate that a chunk needs to be remeshed.
	 *
	 * @param isDirty - If the chunk is dirty.
	 */
	
	inline void setDirty(bool isDirty);
	
	/**
	 * Indicates whether or not the chunk should render,
	 * which will be true if it contains renderable blocks, or false otherwise.
	 *
	 * @returns if the chunk should render.
	 */
	
	inline bool chunkShouldRender() const;
	
	/**
	 * Returns whether or not the chunk is a partial.
	 * A partial is a chunk that has not been fully generated, often only containing structure data.
	 * Ungenerated material is filled with INVALID.
	 *
	 * @returns if the chunk is a partial.
	 */
	
	[[maybe_unused]] inline bool isPartial() const;
	
	/**
	 * Returns whether or not the chunk has been fully generated.
	 * A fully generated chunk is full of its own materials and structures,
	 * but it may still be manipulated later by structures generated nearby.
	 *
	 * @returns if the chunk has been generated.
	 */
	
	inline bool isGenerated() const;
	
	/**
	 * Gets the block ID at the index specified.
	 *
	 * @param ind - The index to get the block at.
	 * @returns the block ID at the requested index.
	 */
	
	inline unsigned int getBlock(unsigned int ind) const;
	
	/**
	 * Sets the block ID at the index specified.
	 *
	 * @param ind - The index to set the block at.
	 * @param blk - The block ID to set the block to.
	 * @returns a boolean indicating if the block replaced a *different* block.
	 */
	
	bool setBlock(unsigned int ind, unsigned int blk);
	
	/**
	 * Gets the block ID at the requested local position.
	 *
	 * @param pos - The position to get the block at.
	 * @returns the block ID at the requested position.
	 */
	
	inline unsigned int getBlock(const glm::ivec3& pos) const;
	
	/**
	 * Sets the block ID at the requested local position.
	 *
	 * @param pos - The position to set the block at.
	 * @param blk - The block ID to set the block to.
	 * @returns a boolean indicating if the block replaced a *different* block.
	 */
	
	inline bool setBlock(const glm::ivec3& pos, unsigned int blk);
	
	/**
	 * Gets the biome ID at the index specified.
	 *
	 * @param ind - The index to get the block at.
	 * @returns the biome ID of the biome at the requested index.
	 */
	
	inline unsigned short getBiome(unsigned int ind) const;
	
	/**
	 * Sets the biome ID at the index specified.
	 *
	 * @param ind - The index to set the biome at.
	 * @param blk - The biome ID to set the biome to.
	 * @returns a boolean indicating if the biome replaced a *different* biome.
	 */
	
	inline bool setBiome(unsigned int ind, unsigned short bio);
	
	/**
	 * Gets the biome ID at the local position specified.
	 *
	 * @param pos - The position to get the block at.
	 * @returns the biome ID of the biome at the requested index.
	 */
	
	inline unsigned short getBiome(const glm::ivec3& pos) const;
	
	/**
	 * Sets the biome ID at the local position specified.
	 *
	 * @param pos - The position to set the biome at.
	 * @param blk - The biome ID to set the biome to.
	 * @returns a boolean indicating if the biome replaced a *different* biome.
	 */
	
	inline bool setBiome(const glm::ivec3& pos, unsigned short bio);
	
	/**
	 * Returns a reference to the chunk's raw blocks array.
	 *
	 * @returns a const reference to the chunk's internal block RIE array.
	 */
	
	const std::vector<unsigned int>& cGetBlocks() const;
	
	/**
	 * Returns a reference to the chunk's raw biomes array.
	 *
	 * @returns a const reference to the chunk's internal biome RIE array.
	 */
	
	const std::vector<unsigned short>& cGetBiomes() const;
	
	/**
	 * Gets the light value at the specified index.
	 *
	 * @param ind - The index to get the light values at.
	 * @returns a four dimensional vector in the format R, G, B, S, with the light values at the specified index.
	 */
	
	inline glm::ivec4 getLight(unsigned int ind);
	
	/**
	 * Sets the light value at the specified index to the vector specified.
	 *
	 * @param ind - The index to set the light values at.
	 * @param light - a four dimensional vector in the format R, G, B, S, with the desired light values.
	 */
	
	inline void setLight(unsigned int ind, glm::ivec4 light);
	
	/**
	 * Gets a single channel's light value at the specified index.
	 *
	 * @param ind - The index to get the light value at.
	 * @param channel - The channel as a char, where 0 = red, 1 = green, 2 = blue, 3 = sunlight.
	 * @returns the light value of the specified channel and index.
	 */
	
	inline unsigned char getLight(unsigned int ind, unsigned char channel);
	
	/**
	 * Sets a single channel's light value at the specified index.
	 *
	 * @param ind - The index to set the light value at.
	 * @param channel - The channel as a char, where 0 = red, 1 = green, 2 = blue, 3 = sunlight.
	 * @returns the light value to set.
	 */
	
	inline void setLight(unsigned int ind, unsigned char channel, unsigned char light);
	
	/**
	 * Combines a chunk's blocks with another's, which may be a partial.
	 * The other chunk's blocks will take priority, but INVALID will be ignored.
	 * Will update the chunk's state to generated one of the two was already generated.
	 *
	 * @param o - The chunk to combine this one with.
	 */
	
	void combineWith(std::shared_ptr<Chunk> o);
	
	/**
	 * Serializes the chunk for sending over the network.
	 *
	 * @returns a packet string containing the chunk's data.
	 */
	
	std::string serialize();
	
	/**
	 * Deserialize chunk data into this chunk.
	 *
	 * @param d - A deserializer, whose current index is the start of a serialized chunk string.
	 */
	
	void deserialize(Deserializer& d);

private:
	
	/**
	 * Gets the sunlight intensity at the specified index.
	 *
	 * @param ind - The index to get the sunlight at.
	 * @returns the sunlight intensity as a char at the specified index.
	 */
	
	inline unsigned char getSunlight(unsigned int ind);
	
	/**
	 * Sets the sunlight intensity at the specified index.
	 *
	 * @param ind - The index to set the sunlight at.
	 * @param val - The value to set the sunlight to, which must range from 0 - 15.
	 */
	
	inline void setSunlight(unsigned int ind, unsigned char val);
	
	/**
	 * Updates the internal Renderable Blocks count, which determines if a chunk should render.
	 * A renderable block is anything except for AIR and INVALID.
	 */
	
	void countRenderableBlocks();
	
	State state = State::EMPTY;
	glm::ivec3 pos {};
	
	bool dirty = true;
	bool shouldRender = true;
	unsigned short renderableBlocks = 0;
	
	std::vector<unsigned int> blocks { 0, 0 };
	std::vector<unsigned short> biomes { 0, 0 };
	
	std::array<SunLight, 2048> sunLight {};
	std::array<BlockLight, 4096> blockLight {};
};

#include "Chunk.inl"