#pragma once

#include <mutex>
#include <array>
#include <vector>
#include <memory>
#include <glm/vec3.hpp>

#include "util/Space.h"

class Deserializer;

class ChunkException : public std::exception {
private:
	glm::ivec3 pos;
	std::string errorMessage;
	
public:
	ChunkException(glm::ivec3 pos, std::string errorMessage):
		pos(pos), errorMessage(errorMessage) {}
	
	const char* what() const throw() {
		return errorMessage.c_str();
	}
};

/**
 * A block chunk that stores a 16^3 region of world data.
 * Can be compressed, any attempts to access data in the chunk will decompress it,
 * and the dimension will automatically compress inactive chunks.
 */

class Chunk {
public:
	friend class MapGen;
	
	/** An enum for the compression state of the chunk. */
	enum class CompressionState { COMPRESSED, DECOMPRESSED };
	
	/** An enum for indicating the generation state of a Chunk. */
	enum class GenerationState { EMPTY, PARTIAL, GENERATED };
	
	/**
	 * A struct for storing Block lighting at a position.
	 * Uses a bitfield, where each light channel is allocated 5 bits.
	 * Each channel can have an intensity from 0 (none), to 31 (full).
	 */
	
	struct BlockLight {
		/** RGB values, and an extra bit, to make the struct an even 2 bytes. */
		unsigned char r: 5, g: 5, b: 5, : 1;
	};
	
	/** A union for accessing a short as a BlockLight struct, or vice versa. */
	typedef union {
		short sh;
		BlockLight b;
	} blocklight_union;
	
	/**
	 * A struct for storing Sunlight at two positions. Ranges from 0 (none) to 15 (full),
	 * so two values can be stored in one byte. Sunlight with a value of 15 will
	 * cascade downwards infinitely without losing intensity.
	 */
	
	struct SunLight {
		/** The two sunlight values stored in a struct. */
		unsigned char a: 4, b: 4;
	};
	
	/** A union for accessing a char as a Sunlight struct, or vice versa. */
	typedef union {
		char ch;
		SunLight s;
	} sunlight_union;
	
	/** Copy constructor, clones the chunk. */
	Chunk(const Chunk& o);
	
	/** Basic chunk constructor, initializes an empty, ungenerated chunk. */
	Chunk(glm::ivec3 pos = glm::ivec3(0), bool partial = false);
	
	/** Creates a chunk with the compressed data specified. */
	Chunk(const std::string& data);
	
	/** Destroys chunk data pointer. */
	~Chunk();
	
	/** Returns the position of the chunk. */
	inline glm::ivec3 getPos() const;
	
	/** Sets the position of the chunk. */
	inline void setPos(glm::ivec3 newPos);
	
	/**
	 * Returns the chunk's dirty state, which is whether it needs to be remeshed or not.
	 * This value is only set through other classes using setDirty.
	 */
	
	inline bool isDirty() const;
	
	/** Set a chunks dirty state, which indicates that it needs to be remeshed. */
	inline void setDirty(bool isDirty);
	
	/** Returns a boolean indicating if the chunk needs a visual representation in the world. */
	inline bool chunkShouldRender() const;
	
	/** Returns a boolean indicating if the chunk is a partial. */
	// TODO: Partials should not be stored as chunks and should go away.
	[[maybe_unused]] inline bool isPartial() const;
	
	/**
	 * Returns whether or not the chunk has been fully generated.
	 * A fully generated chunk is full of its own materials and structures,
	 * but it may still be manipulated later by structures generated nearby.
	 */
	
	inline bool isGenerated() const;
	
	/** Returns the block ID at the index specified. */
	inline unsigned int getBlock(unsigned int ind) const;
	
	/**
	 * Sets the block ID at the index specified.
	 * @returns a boolean indicating if the newly placed block is different than the old one.
	 */
	
	bool setBlock(unsigned int ind, unsigned int blk);
	
	/** Returns the block ID at the position specified, wrapping to local coordinates. */
	inline unsigned int getBlock(const glm::ivec3& pos) const;
	
	/**
	 * Sets the block ID at the position specified, wrapping to local coordinates.
	 * @returns a boolean indicating if the newly placed block is different than the old one.
	 */
	
	inline bool setBlock(const glm::ivec3& pos, unsigned int blk);
	
	/** Gets the biome ID at the index specified. */
	inline unsigned short getBiome(unsigned int ind) const;
	
	/**
	 * Sets the biome ID at the index specified.
	 * @returns a boolean indicating if the newly placed biome is different than the old one.
	 */
	
	inline bool setBiome(unsigned int ind, unsigned short bio);
	
	/** Returns the biome ID at the position specified, wrapping to local coordinates. */
	inline unsigned short getBiome(const glm::ivec3& pos) const;
	
	/**
	 * Sets the biome ID at the position specified, wrapping to local coordinates.
	 * @returns a boolean indicating if the newly placed biome is different than the old one.
	 */
	
	inline bool setBiome(const glm::ivec3& pos, unsigned short bio);
	
	/** Returns a constant reference to the chunk's raw blocks array. */
	const std::array<unsigned int, 4096>& getBlocksArray() const;
	
	/** Returns a constant reference to the chunk's raw biomes array. */
	const std::array<unsigned short, 4096>& getBiomesArray() const;
	
	/** Returns the light value at the specified index as a vector in R, G, B, Sunlight format. */
	inline glm::ivec4 getLight(unsigned int ind);
	
	/** Sets the light value at the specified index to the vector specified in R, G, B, Sunlight format. */
	inline void setLight(unsigned int ind, glm::ivec4 light);
	
	/** Returns the specified channel of the light value at the specified index. */
	inline unsigned char getLight(unsigned int ind, unsigned char channel);
	
	/** Sets the specified channel of the light value at the specified index. */
	inline void setLight(unsigned int ind, unsigned char channel, unsigned char light);
	
	/**
	 * Combines a chunk's blocks with another's, which may be a partial.
	 * The other chunk's blocks will take priority, but INVALID will be ignored.
	 * Will update the chunk's state to generated one of the two was already generated.
	 */
	
	void combineWith(std::shared_ptr<Chunk> o);
	
	/** Compresses the chunk, returning a string representing it. */
	std::string compress();
	
	/** Decompresses a compressed chunk string, or itself. */
	void decompress(const std::string& data = "");

private:
	/** Internal data of a decompressed chunk. */
	struct ChunkData {
		/** Internal block data. */
		std::array<unsigned int, 4096> blocks {};
		
		/** Internal biome data. */
		std::array<unsigned short, 4096> biomes {};
		
		/** Internal sunlight data. */
		std::array<SunLight, 2048> sunLight {};
		
		/** Internal blocklight data. */
		std::array<BlockLight, 4096> blockLight {};
	};
	
	/** Throws an exception if the chunk is compressed. */
	inline void assertDecompressed() const;
	
	/** Gets the sunlight intensity at the specified index. */
	inline unsigned char getSunlight(unsigned int ind);
	
	/** Sets the sunlight intensity at the specified index. */
	inline void setSunlight(unsigned int ind, unsigned char val);
	
	/**
	 * Updates the internal Renderable Blocks count, which determines if a chunk should render.
	 * A renderable block is anything except for AIR and INVALID.
	 */
	
	void countRenderableBlocks();
	
	/** Whether or not the chunk is compressed. */
	CompressionState compressionState = CompressionState::COMPRESSED;
	
	/** Whether or not the chunk is generated. */
	GenerationState generationState = GenerationState::EMPTY;
	
	/** The position of the chunk in its dimension. */
	glm::ivec3 pos {};
	
	/** Whether or not the chunk needs to be remeshed. */
	bool dirty = true;
	
	/** The number of non-transparent blocks in the chunk. */
	unsigned short renderableBlocks = 0;

	/** Internal decompressed chunk data. */
	ChunkData* d = nullptr;
	
	/** Internal compressed chunk data. */
	std::string c = "";
};

#include "Chunk.inl"