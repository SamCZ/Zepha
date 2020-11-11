
#include "Chunk.h"

glm::ivec3 Chunk::getPos() const {
	return pos;
}

void Chunk::setPos(glm::ivec3 newPos) {
	pos = newPos;
}

bool Chunk::isDirty() const {
	return dirty;
}

void Chunk::setDirty(bool isDirty) {
	dirty = isDirty;
}

bool Chunk::chunkShouldRender() const {
	return shouldRender;
}

[[maybe_unused]] bool Chunk::isPartial() const {
	return state == State::PARTIAL;
}

bool Chunk::isGenerated() const {
	return state == State::GENERATED;
}

inline unsigned int Chunk::getBlock(unsigned int ind) const {
	if (ind >= 4096) return 0; // Invalid
	return RIE::read<unsigned int>(ind, blocks, 4096);
}

inline unsigned int Chunk::getBlock(const glm::ivec3& reqPos) const {
	if (reqPos.x > 15 || reqPos.x < 0 || reqPos.y > 15 || reqPos.y < 0 || reqPos.z > 15 || reqPos.z < 0) return 0;
	return getBlock(Space::Block::index(reqPos));
}

inline bool Chunk::setBlock(const glm::ivec3& newPos, unsigned int blk) {
	if (newPos.x > 15 || newPos.x < 0 || newPos.y > 15 || newPos.y < 0 || newPos.z > 15 || newPos.z < 0) return false;
	return setBlock(Space::Block::index(newPos), blk);
}

inline unsigned short Chunk::getBiome(unsigned int ind) const {
	if (ind >= 4096) return 0; // Invalid
	return RIE::read<unsigned short>(ind, biomes, 4096);
}

inline unsigned short Chunk::getBiome(const glm::ivec3& reqPos) const {
	if (reqPos.x > 15 || reqPos.x < 0 || reqPos.y > 15 || reqPos.y < 0 || reqPos.z > 15 || reqPos.z < 0) return 0;
	return getBiome(Space::Block::index(reqPos));
}

inline bool Chunk::setBiome(unsigned int ind, unsigned short bio) {
	return RIE::write(ind, bio, biomes, 4096);
}

inline bool Chunk::setBiome(const glm::ivec3& newPos, unsigned short bio) {
	if (newPos.x > 15 || newPos.x < 0 || newPos.y > 15 || newPos.y < 0 || newPos.z > 15 || newPos.z < 0) return false;
	return setBiome(Space::Block::index(newPos), bio);
}

inline glm::ivec4 Chunk::getLight(unsigned int ind) {
	return { blockLight[ind].r, blockLight[ind].g, blockLight[ind].b, getSunlight(ind) };
}

inline void Chunk::setLight(unsigned int ind, glm::ivec4 light) {
	blockLight[ind].r = static_cast<unsigned char>(light.x);
	blockLight[ind].g = static_cast<unsigned char>(light.y);
	blockLight[ind].b = static_cast<unsigned char>(light.z);
	setSunlight(ind, static_cast<unsigned char>(light.w));
}

inline unsigned char Chunk::getLight(unsigned int ind, unsigned char channel) {
	return channel == 0 ? blockLight[ind].r :
		channel == 1 ? blockLight[ind].g :
			channel == 2 ? blockLight[ind].b :
				getSunlight(ind);
}

inline void Chunk::setLight(unsigned int ind, unsigned char channel, unsigned char light) {
	channel == 0 ? blockLight[ind].r = light :
		channel == 1 ? blockLight[ind].g = light :
			channel == 2 ? blockLight[ind].b = light :
				(setSunlight(ind, light), 0);
}

inline unsigned char Chunk::getSunlight(unsigned int ind) {
	if (ind % 2 == 0) return sunLight[ind / 2].a;
	else return sunLight[ind / 2].b;
}

inline void Chunk::setSunlight(unsigned int ind, unsigned char val) {
	if (ind % 2 == 0) sunLight[ind / 2].a = val;
	else sunLight[ind / 2].b = val;
}