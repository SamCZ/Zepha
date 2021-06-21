
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
	return renderableBlocks > 0;
}

[[maybe_unused]] bool Chunk::isPartial() const {
	return generationState == GenerationState::PARTIAL;
}

bool Chunk::isGenerated() const {
	return generationState == GenerationState::GENERATED;
}

inline unsigned int Chunk::getBlock(unsigned int ind) const {
	assertDecompressed();
	if (ind >= 4096) throw ChunkException(pos, "Index out of range.");
	return d->blocks[ind];
}

inline unsigned int Chunk::getBlock(const glm::ivec3& reqPos) const {
	return getBlock(Space::Block::index(reqPos));
}

inline bool Chunk::setBlock(const glm::ivec3& newPos, unsigned int blk) {
	return setBlock(Space::Block::index(newPos), blk);
}

inline unsigned short Chunk::getBiome(unsigned int ind) const {
	assertDecompressed();
	if (ind >= 4096) throw ChunkException(pos, "Index out of range.");
	return d->biomes[ind];
}

inline unsigned short Chunk::getBiome(const glm::ivec3& reqPos) const {
	return getBiome(Space::Block::index(reqPos));
}

inline bool Chunk::setBiome(unsigned int ind, unsigned short bio) {
	assertDecompressed();
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	
	if (d->biomes[ind] == bio) return false;
	d->biomes[ind] = bio;
	return true;
}

inline bool Chunk::setBiome(const glm::ivec3& newPos, unsigned short bio) {
	return setBiome(Space::Block::index(newPos), bio);
}

inline glm::ivec4 Chunk::getLight(unsigned int ind) {
	assertDecompressed();
	return { d->blockLight[ind].r, d->blockLight[ind].g, d->blockLight[ind].b, getSunlight(ind) };
}

inline void Chunk::setLight(unsigned int ind, glm::ivec4 light) {
	assertDecompressed();
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	if (light.x > 31 || light.y > 31 || light.z > 31 || light.w > 15)
		throw ChunkException(pos, "Light value out of range.");
	
	d->blockLight[ind].r = static_cast<unsigned char>(light.x);
	d->blockLight[ind].g = static_cast<unsigned char>(light.y);
	d->blockLight[ind].b = static_cast<unsigned char>(light.z);
	setSunlight(ind, static_cast<unsigned char>(light.w));
}

inline unsigned char Chunk::getLight(unsigned int ind, unsigned char channel) {
	assertDecompressed();
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	
	return channel == 0 ? d->blockLight[ind].r :
		   channel == 1 ? d->blockLight[ind].g :
		   channel == 2 ? d->blockLight[ind].b :
		                  getSunlight(ind);
}

inline void Chunk::setLight(unsigned int ind, unsigned char channel, unsigned char light) {
	assertDecompressed();
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	if ((channel < 4 && light > 31) || (channel == 4 && light > 15))
		throw ChunkException(pos, "Light value out of range.");
	
	if (channel == 0) d->blockLight[ind].r = light;
	else if (channel == 1) d->blockLight[ind].g = light;
	else if (channel == 2) d->blockLight[ind].b = light;
	else setSunlight(ind, light);
}

void Chunk::assertDecompressed() const {
	if (compressionState == CompressionState::COMPRESSED)
		throw ChunkException(pos, "Chunk is compressed.");
}

inline unsigned char Chunk::getSunlight(unsigned int ind) {
	if (ind % 2 == 0) return d->sunLight[ind / 2].a;
	else return d->sunLight[ind / 2].b;
}

inline void Chunk::setSunlight(unsigned int ind, unsigned char val) {
	if (ind % 2 == 0) d->sunLight[ind / 2].a = val;
	else d->sunLight[ind / 2].b = val;
}