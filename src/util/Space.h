#pragma once

#include <tgmath.h>

#include "util/Types.h"

/**
 * Static methods to convert between different coordinate spaces,
 * and transform coordinates into indices and vice-versa.
 */

namespace Space {
	const static u16f CHUNK_SIZE = 16;
	const static u16f MAPBLOCK_SIZE = 4;
	const static u16f REGION_SIZE = 4;
	
	const static u16f CHUNK_BLOCK_LENGTH = CHUNK_SIZE;
	const static u16f MAPBLOCK_BLOCK_LENGTH = CHUNK_BLOCK_LENGTH * MAPBLOCK_SIZE;
	const static u16f REGION_BLOCK_LENGTH = MAPBLOCK_BLOCK_LENGTH * REGION_SIZE;
	
	const static u16f MAPBLOCK_CHUNK_LENGTH = MAPBLOCK_SIZE;
	const static u16f REGION_CHUNK_LENGTH = MAPBLOCK_CHUNK_LENGTH * REGION_SIZE;
	
	// Private helper methods
	namespace {
		inline u16vec3 localFromGlobal(const ivec3& pos, u32 size) {
			return u16vec3 {
				size - 1 + (pos.x + (pos.x < 0 ? 1 : 0)) % size,
				size - 1 + (pos.y + (pos.y < 0 ? 1 : 0)) % size,
				size - 1 + (pos.z + (pos.z < 0 ? 1 : 0)) % size
			};
		}
		
		inline ivec3 sectionFromGlobal(ivec3 pos, f32 size) {
			return {
				std::floor(static_cast<float>(pos.x) / size),
				std::floor(static_cast<float>(pos.y) / size),
				std::floor(static_cast<float>(pos.z) / size)
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
			u8vec3 local = MapBlock::relative::toRegion(vec);
			return local.x + REGION_SIZE * (local.y + REGION_SIZE * local.z);
		}
	}
	
	namespace Chunk {
		namespace relative {
			// Get a Chunk's relative position in its MapBlock from its world position.
			static inline u8vec3 toMapBlock(const ivec3& pos) {
				return localFromGlobal(pos, MAPBLOCK_CHUNK_LENGTH);
			}
			
			// Get a Chunk's relative position in its Region from its world position.
			static inline u8vec3 toRegion(const ivec3& pos) {
				return localFromGlobal(pos, REGION_CHUNK_LENGTH);
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
			u8vec3 local = Chunk::relative::toMapBlock(vec);
			return local.x + MAPBLOCK_SIZE * (local.z + MAPBLOCK_SIZE * local.y);
		}
		
		// Return a local vector of an chunk within its mapblock.
		static inline ivec3 fromIndex(u16 ind) {
			u8vec3 vec {};
			
			vec.y = ind / (MAPBLOCK_SIZE * MAPBLOCK_SIZE);
			ind -= (static_cast<int>(vec.y) * MAPBLOCK_SIZE * MAPBLOCK_SIZE);
			vec.z = ind / MAPBLOCK_SIZE;
			vec.x = ind % MAPBLOCK_SIZE;
			
			return vec;
		}
	}
	
	namespace Block {
		namespace relative {
			// Get a Block's relative position to its Chunk from its world position.
			static inline u8vec3 toChunk(const ivec3& pos) {
				return localFromGlobal(pos, CHUNK_BLOCK_LENGTH);
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
			u8vec3 local = Block::relative::toChunk(vec);
			return local.x + CHUNK_SIZE * (local.z + CHUNK_SIZE * local.y);
		}
		
		// Return a local vector of an block within its chunk.
		static inline u8vec3 fromIndex(u16 ind) {
			u8vec3 vec {};
			
			vec.y = ind / (CHUNK_SIZE * CHUNK_SIZE);
			ind -= (static_cast<int>(vec.y) * CHUNK_SIZE * CHUNK_SIZE);
			vec.z = ind / CHUNK_SIZE;
			vec.x = ind % CHUNK_SIZE;
			
			return vec;
		}
	}
}

