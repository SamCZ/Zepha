#pragma once

#include <mutex>

#include "util/Types.h"
#include "util/Space.h"

class Deserializer;

class ChunkException : public std::exception {
private:
	ivec3 pos;
	string errorMessage;
	
public:
	ChunkException(ivec3 pos, string errorMessage):
		pos(pos), errorMessage(errorMessage) {}
	
	const char* what() const throw() {
		return errorMessage.c_str();
	}
};

/**
 * A block chunk that stores a 16^3 region of world data.
 * Can be compressed, any attempts to access data in the chunk will decompressFromString it,
 * and the dimension will automatically compressToString inactive chunks.
 */

class Chunk {
public:
	friend class MapGen;
	
	/** An enum for the compression state of the chunk. */
	enum class CompressionState { COMPRESSED, DECOMPRESSED };
	
	/** An enum for indicating the generation state of a Chunk. */
	enum class GenerationState { EMPTY, PARTIAL, GENERATED };
	
	/** How long the chunk should wait before being compressed, in seconds */
	constexpr static f64 COMPRESS_DELAY = 2;
	
	/**
	 * A struct for storing Block lighting at a position.
	 * Uses a bitfield, where each light channel is allocated 5 bits.
	 * Each channel can have an intensity from 0 (none), to 31 (full).
	 */
	
	struct BlockLight {
		/** RGB values, and an extra bit, to make the struct an even 2 bytes. */
		u8 r: 5, g: 5, b: 5, : 1;
	};
	
	/** A union for accessing a short as a BlockLight struct, or vice versa. */
	typedef union {
		u16 sh;
		BlockLight b;
	} blocklight_union;
	
	/**
	 * A struct for storing Sunlight at two positions. Ranges from 0 (none) to 15 (full),
	 * so two values can be stored in one byte. Sunlight with a value of 15 will
	 * cascade downwards infinitely without losing intensity.
	 */
	
	struct SunLight {
		/** The two sunlight values stored in a struct. */
		u8 a: 4, b: 4;
	};
	
	/** A union for accessing a char as a Sunlight struct, or vice versa. */
	typedef union {
		u8 ch;
		SunLight s;
	} sunlight_union;
	
	/** Copy constructor, clones the chunk. */
	Chunk(const Chunk& o);
	
	/** Basic chunk constructor, initializes an empty, ungenerated chunk. */
	Chunk(ivec3 pos = { 0, 0, 0 }, bool partial = false);
	
	/** Creates a chunk with the compressed data specified. */
	Chunk(const string& data);
	
	/** Returns the position of the chunk. */
	inline ivec3 getPos() const;
	
	/** Sets the position of the chunk. */
	inline void setPos(ivec3 newPos);
	
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
	
	/** Returns a boolean indicating if the chunk is compressed. */
	inline bool isCompressed() const;
	
	/** Returns the block ID at the index specified. */
	inline u16 getBlock(u16 ind);
	
	/**
	 * Sets the block ID at the index specified.
	 * @returns a boolean indicating if the newly placed block is different than the old one.
	 */
	
	bool setBlock(u16 ind, u16 blk);
	
	/** Returns the block ID at the position specified, wrapping to local coordinates. */
	inline u16 getBlock(const ivec3& pos);
	
	/**
	 * Sets the block ID at the position specified, wrapping to local coordinates.
	 * @returns a boolean indicating if the newly placed block is different than the old one.
	 */
	
	inline bool setBlock(const ivec3& pos, u16 blk);
	
	/** Gets the biome ID at the index specified. */
	inline u16 getBiome(u16 ind);
	
	/**
	 * Sets the biome ID at the index specified.
	 * @returns a boolean indicating if the newly placed biome is different than the old one.
	 */
	
	inline bool setBiome(u16 ind, u16 bio);
	
	/** Returns the biome ID at the position specified, wrapping to local coordinates. */
	inline u16 getBiome(const ivec3& pos);
	
	/**
	 * Sets the biome ID at the position specified, wrapping to local coordinates.
	 * @returns a boolean indicating if the newly placed biome is different than the old one.
	 */
	
	inline bool setBiome(const ivec3& pos, u16 bio);
	
	/** Returns a constant reference to the chunk's raw blocks array. */
	const array<u16, 4096>& getBlocksArray();
	
	/** Returns a constant reference to the chunk's raw biomes array. */
	const array<u16, 4096>& getBiomesArray();
	
	/** Returns the light value at the specified index as a vector in R, G, B, Sunlight format. */
	inline u8vec4 getLight(u16 ind);
	
	/** Sets the light value at the specified index to the vector specified in R, G, B, Sunlight format. */
	inline void setLight(u16 ind, u8vec4 light);
	
	/** Returns the specified channel of the light value at the specified index. */
	inline u8 getLight(u16 ind, u8 channel);
	
	/** Sets the specified channel of the light value at the specified index. */
	inline void setLight(u16 ind, u8 channel, u8 light);
	
	/**
	 * Combines a chunk's blocks with another's, which may be a partial.
	 * The other chunk's blocks will take priority, but INVALID will be ignored.
	 * Will update the chunk's state to generated one of the two was already generated.
	 */
	
	void combineWith(sptr<Chunk> o);
	
	/** Compresses the chunk without modifying it, returning a string representing it. */
	string compressToString() const;
	
	/** Compresses the chunk in-place, returning a read-only reference to the compressed data. */
	inline const string& compress();
	
	/** Decompresses a compressed chunk string into the chunk itself. */
	void decompressFromString(const string& data);
	
	/** Deompresses the chunk in-place from internal data. */
	inline void decompress();
	
	/** Decompresses the chunk if it isn't already, and marks the current time of use. */
	inline void useDecompressed();
	
	/** Compresses the chunk if it hasn't been used for COMPRESS_DELAY seconds. */
	void compressIfIdle();

private:
	/** Internal data of a decompressed chunk. */
	struct ChunkData {
		ChunkData() = default;
		ChunkData(const ChunkData& o) : blocks(o.blocks), biomes(o.biomes),
			sunLight(o.sunLight), blockLight(o.blockLight) {}
			
		/** Internal block data. */
		array<u16, 4096> blocks {};
		
		/** Internal biome data. */
		array<u16, 4096> biomes {};
		
		/** Internal sunlight data. */
		array<SunLight, 2048> sunLight {};
		
		/** Internal blocklight data. */
		array<BlockLight, 4096> blockLight {};
	};
	
	/** Gets the sunlight intensity at the specified index. */
	inline u8 getSunlight(u16 ind);
	
	/** Sets the sunlight intensity at the specified index. */
	inline void setSunlight(u16 ind, u8 val);
	
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
	ivec3 pos {};
	
	/** Whether or not the chunk needs to be remeshed. */
	bool dirty = true;
	
	/** The last time the uncompressed chunk is used. */
	time_t lastUsed = 0;
	
	/** The number of non-transparent blocks in the chunk. */
	u16 renderableBlocks = 1; // TODO: Don't hack this.

	/** Internal decompressed chunk data. */
	uptr<ChunkData> d = nullptr;
	
	/** Internal compressed chunk data. */
	string c = "";
};

#include "Chunk.inl"