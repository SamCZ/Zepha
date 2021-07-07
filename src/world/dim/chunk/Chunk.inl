#include "Chunk.h"

ivec3 Chunk::getPos() const {
	return pos;
}

void Chunk::setPos(ivec3 newPos) {
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

bool Chunk::isCompressed() const {
	return compressionState == CompressionState::COMPRESSED;
}

inline u16 Chunk::getBlock(u16 ind) {
	useDecompressed();
	
	if (ind >= 4096) throw ChunkException(pos, "Index out of range.");
	return d->blocks[ind];
}

inline u16 Chunk::getBlock(const ivec3& reqPos) {
	return getBlock(Space::Block::index(reqPos));
}

inline bool Chunk::setBlock(const ivec3& newPos, u16 blk) {
	return setBlock(Space::Block::index(newPos), blk);
}

inline u16 Chunk::getBiome(u16 ind) {
	useDecompressed();
	
	if (ind >= 4096) throw ChunkException(pos, "Index out of range.");
	return d->biomes[ind];
}

inline u16 Chunk::getBiome(const ivec3& reqPos) {
	return getBiome(Space::Block::index(reqPos));
}

inline bool Chunk::setBiome(u16 ind, u16 bio) {
	useDecompressed();
	
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	
	if (d->biomes[ind] == bio) return false;
	d->biomes[ind] = bio;
	return true;
}

inline bool Chunk::setBiome(const ivec3& newPos, u16 bio) {
	return setBiome(Space::Block::index(newPos), bio);
}

inline u8vec4 Chunk::getLight(u16 ind) {
	useDecompressed();
	
	return { d->blockLight[ind].r, d->blockLight[ind].g, d->blockLight[ind].b, getSunlight(ind) };
}

inline void Chunk::setLight(u16 ind, u8vec4 light) {
	useDecompressed();
	
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	if (light.x > 31 || light.y > 31 || light.z > 31 || light.w > 15)
		throw ChunkException(pos, "Light value out of range.");
	
	d->blockLight[ind].r = light.x;
	d->blockLight[ind].g = light.y;
	d->blockLight[ind].b = light.z;
	setSunlight(ind, light.w);
}

inline u8 Chunk::getLight(u16 ind, u8 channel) {
	useDecompressed();
	
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	
	return channel == 0 ? d->blockLight[ind].r :
		   channel == 1 ? d->blockLight[ind].g :
		   channel == 2 ? d->blockLight[ind].b :
		                  getSunlight(ind);
}

inline void Chunk::setLight(u16 ind, u8 channel, u8 light) {
	useDecompressed();
	
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	if ((channel < 4 && light > 31) || (channel == 4 && light > 15))
		throw ChunkException(pos, "Light value out of range.");
	
	if (channel == 0) d->blockLight[ind].r = light;
	else if (channel == 1) d->blockLight[ind].g = light;
	else if (channel == 2) d->blockLight[ind].b = light;
	else setSunlight(ind, light);
}

const string& Chunk::compress() {
	if (isCompressed()) throw ChunkException(pos, "Chunk is compressed.");
	c = compressToString();
	d = nullptr;
	compressionState = CompressionState::COMPRESSED;
	return c;
}

void Chunk::decompress() {
	if (!isCompressed()) throw ChunkException(pos, "Chunk is not compressed.");
	d = nullptr;
	decompressFromString(c);
}

void Chunk::useDecompressed() {
	lastUsed = time(0);
	if (isCompressed()) decompress();
}

inline u8 Chunk::getSunlight(u16 ind) {
	if (ind % 2 == 0) return d->sunLight[ind / 2].a;
	else return d->sunLight[ind / 2].b;
}

inline void Chunk::setSunlight(u16 ind, u8 val) {
	if (ind % 2 == 0) d->sunLight[ind / 2].a = val;
	else d->sunLight[ind / 2].b = val;
}