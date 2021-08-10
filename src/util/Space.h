#pragma once

#include <tgmath.h>

#include "util/Types.h"

/**
 * Static methods to convert between different coordinate spaces,
 * and transform coordinates into indices and vice-versa.
 */

namespace Space {
	const static i16f CHUNK_SIZE = 16;
	const static i16f MAPBLOCK_SIZE = 4;
	const static i16f REGION_SIZE = 4;
	
	const static i16f CHUNK_BLOCK_LENGTH = CHUNK_SIZE;
	const static i16f MAPBLOCK_BLOCK_LENGTH = CHUNK_BLOCK_LENGTH * MAPBLOCK_SIZE;
	const static i16f REGION_BLOCK_LENGTH = MAPBLOCK_BLOCK_LENGTH * REGION_SIZE;
	
	const static i16f MAPBLOCK_CHUNK_LENGTH = MAPBLOCK_SIZE;
	const static i16f REGION_CHUNK_LENGTH = MAPBLOCK_CHUNK_LENGTH * REGION_SIZE;
	
	// Get the index of a position vector in a cube.
	static inline u32 posToIndex(const ivec3& pos, const u32 size) {
		return pos.x + size * (pos.z + size * pos.y);
	}
	
	// Return a position vector from an index of a cube.
	static inline ivec3 indexToPos(u32 ind, const u32 size) {
		ivec3 vec {};
		vec.y = ind / pow(size, 2);
		ind -= static_cast<u32>(vec.y) * pow(size, 2);
		vec.z = ind / size;
		vec.x = ind % size;
		return vec;
	}
	
	// Private helper methods
	namespace {
		inline u16vec3 localFromGlobal(const ivec3& pos, i16 size) {
			return u16vec3 {
				pos.x < 0 ? size - 1 + (pos.x + 1) % size : pos.x % size,
				pos.y < 0 ? size - 1 + (pos.y + 1) % size : pos.y % size,
				pos.z < 0 ? size - 1 + (pos.z + 1) % size : pos.z % size
			};
		}
		
		inline ivec3 sectionFromGlobal(ivec3 pos, f32 size) {
			return {
				std::floor(static_cast<f32>(pos.x) / size),
				std::floor(static_cast<f32>(pos.y) / size),
				std::floor(static_cast<f32>(pos.z) / size)
			};
		}
	}
	
	namespace Region {
		namespace world {
			// Get a Region world position from a MapBlock's world position.
			static inline ivec3 fromMapBlock(const ivec3& pos) {
				return sectionFromGlobal(pos, REGION_SIZE);
			}
			
			// Get a Region world position from a Chunk's world position.
			static inline ivec3 fromChunk(const ivec3& pos) {
				return sectionFromGlobal(pos, REGION_CHUNK_LENGTH);
			}
			
			// Get a Region world position from a Block's world position.
			static inline ivec3 fromBlock(const ivec3& pos) {
				return sectionFromGlobal(pos, REGION_BLOCK_LENGTH);
			}
		}
	}
	
	namespace MapBlock {
		namespace relative {
			// Get a MapBlock's relative position in its Region from its world position.
			static inline u8vec3 toRegion(const ivec3& pos) {
				return localFromGlobal(pos, MAPBLOCK_SIZE);
			}
		}
		
		namespace world {
			// Get a MapBlock world position from a Region's world position.
			static inline ivec3 fromRegion(const ivec3& pos) {
				return sectionFromGlobal(pos, 1.f / REGION_SIZE);
			}
			
			// Get a MapBlock world position from a Chunk's world position.
			static inline ivec3 fromChunk(const ivec3& pos) {
				return sectionFromGlobal(pos, MAPBLOCK_SIZE);
			}
			
			// Get a MapBlock world position from a Block's world position.
			static inline ivec3 fromBlock(const ivec3& pos) {
				return sectionFromGlobal(pos, MAPBLOCK_BLOCK_LENGTH);
			}
		}
		
		// Get the index of a MapBlock within its Region from its local or world position.
		static inline u16 index(const ivec3& vec) {
			return Space::posToIndex(MapBlock::relative::toRegion(vec), REGION_SIZE);
		}
	}
	
	namespace Chunk {
		namespace relative {
			// Get a Chunk's relative position in its MapBlock from its world position.
			static inline u8vec3 toMapBlock(const ivec3& pos) {
				return u8vec3(localFromGlobal(pos, MAPBLOCK_CHUNK_LENGTH));
			}
			
			// Get a Chunk's relative position in its Region from its world position.
			static inline u8vec3 toRegion(const ivec3& pos) {
				return u8vec3(localFromGlobal(pos, REGION_CHUNK_LENGTH));
			}
		}
		
		namespace world {
			// Get a Chunk world position from a Regions's world position.
			static inline ivec3 fromRegion(const ivec3& pos) {
				return sectionFromGlobal(pos, 1.f / REGION_CHUNK_LENGTH);
			}
			
			// Get a Chunk world position from a MapBlock's world position.
			static inline ivec3 fromMapBlock(const ivec3& pos) {
				return sectionFromGlobal(pos, 1.f / MAPBLOCK_CHUNK_LENGTH);
			}
			
			// Get a Chunk world position from a Block's world position.
			static inline ivec3 fromBlock(const ivec3& pos) {
				return sectionFromGlobal(pos, CHUNK_BLOCK_LENGTH);
			}
		}
		
		// Get the index of a Chunk within its MapBlock from its local or world position.
		static inline u16 index(const glm::ivec3& vec) {
			return Space::posToIndex(Chunk::relative::toMapBlock(vec), MAPBLOCK_SIZE);
		}
		
		// Return a local vector of an chunk within its mapblock.
		static inline ivec3 fromIndex(u16 ind) {
			return Space::indexToPos(ind, MAPBLOCK_SIZE);
		}
	}
	
	namespace Block {
		namespace relative {
			// Get a Block's relative position to its Chunk from its world position.
			static inline u8vec3 toChunk(const ivec3& pos) {
				return u8vec3(localFromGlobal(pos, CHUNK_BLOCK_LENGTH));
			}
			
			// Get a Block's relative position to its MapBlock from its world position.
			static inline u16vec3 toMapBlock(const ivec3& pos) {
				return localFromGlobal(pos, MAPBLOCK_BLOCK_LENGTH);
			}
			
			// Get a Block's relative position in its Region from its world position.
			static inline u16vec3 toRegion(const ivec3& pos) {
				return localFromGlobal(pos, REGION_BLOCK_LENGTH);
			}
			
			namespace World {
				// Get a Block world position from a Regions's world position.
				static inline ivec3 fromRegion(const ivec3& pos) {
					return sectionFromGlobal(pos, 1.f / REGION_BLOCK_LENGTH);
				}
				
				// Get a Block world position from a MapBlock's world position.
				static inline ivec3 fromMapBlock(const ivec3& pos) {
					return sectionFromGlobal(pos, 1.f / MAPBLOCK_BLOCK_LENGTH);
				}
				
				// Get a Block world position from a Chunk's world position.
				static inline ivec3 fromChunk(const ivec3& pos) {
					return sectionFromGlobal(pos, CHUNK_BLOCK_LENGTH);
				}
			}
		}
		
		// Get the index of a Block within its Chunk from its local or world position.
		static inline u16 index(const ivec3& vec) {
			return Space::posToIndex(Block::relative::toChunk(vec), CHUNK_SIZE);
		}
		
		// Return a local vector of an block within its chunk.
		static inline u8vec3 fromIndex(u16 ind) {
			return Space::indexToPos(ind, CHUNK_SIZE);
		}
	}
}

