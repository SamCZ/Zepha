#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>
#include <util/RIE.h>

#include "Chunk.h"

#include "util/net/Serializer.h"
#include "util/net/Deserializer.h"
#include "game/atlas/DefinitionAtlas.h"

Chunk::Chunk(const Chunk& o) :
	pos(o.pos),
	dirty(true),
	renderableBlocks(o.renderableBlocks),
	generationState(o.generationState),
	compressionState(o.compressionState),
	c(o.c) {
	if (o.d != nullptr) d = make_unique<ChunkData>(*o.d.get());
}

Chunk::Chunk(ivec3 pos, bool partial) :
	pos(pos),
	d(new ChunkData()),
	compressionState(CompressionState::DECOMPRESSED),
	generationState(partial ? GenerationState::PARTIAL : GenerationState::EMPTY) {}

Chunk::Chunk(ivec3 pos, const string& data) : pos(pos), c(data) {}

bool Chunk::setBlock(u16 ind, u16 blk) {
	useDecompressed();
	
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	
	if (d->blocks[ind] == blk) return false;
	d->blocks[ind] = blk;
	
	if (blk == DefinitionAtlas::AIR) renderableBlocks = std::max(renderableBlocks - 1, 0);
	else renderableBlocks++;
	
	return true;
}

const array<u16, 4096>& Chunk::getBlocksArray() {
	useDecompressed();
	return d->blocks;
}

const array<u16, 4096>& Chunk::getBiomesArray() {
	useDecompressed();
	return d->biomes;
}

void Chunk::combineWith(sptr<Chunk> o) {
	for (u16 i = 0; i < 4096; i++)
		if (o->getBlock(i) > DefinitionAtlas::INVALID) setBlock(i, o->getBlock(i));
	
	if (generationState == GenerationState::GENERATED || o->isGenerated()) {
		generationState = GenerationState::GENERATED;
		countRenderableBlocks();
	}
	
	else generationState = GenerationState::PARTIAL;
}

string Chunk::compressToString() const {
	if (isCompressed()) return c;
	
	Serializer s;
	
	std::vector<u16> blocksRIE = {};
	std::vector<u16> biomesRIE = {};
	
	RIE::encode<u16, 4096>(d->blocks, blocksRIE);
	RIE::encode<u16, 4096>(d->biomes, biomesRIE);
	
	string temp = Serializer()
		.append(blocksRIE)
		.append(biomesRIE)
		.append(d->blockLight)
		.append(d->sunLight).data;
	
	s.append(pos).append(gzip::compress(temp.data(), temp.size()));
	
	return s.data;
}

void Chunk::decompressFromString(const string& data) {
	
	Deserializer raw(data);
	pos = raw.read<ivec3>();
	
	string gzipped = raw.read<string>();
	if (!gzip::is_compressed(gzipped.data(), gzipped.length()))
		throw ChunkException(pos, "Chunk contains invalid gzipped data.");

	c = "";
	d = make_unique<ChunkData>();
	compressionState = CompressionState::DECOMPRESSED;

	vec<u16> blocksRIE = {};
	vec<u16> biomesRIE = {};

	Deserializer(gzip::decompress(gzipped.data(), gzipped.length()))
		.read<vec<u16>>(blocksRIE)
		.read<vec<u16>>(biomesRIE)
		.read<array<BlockLight, 4096>>(d->blockLight)
		.read<array<SunLight, 2048>>(d->sunLight);

	RIE::expand<u16, 4096>(blocksRIE, d->blocks);
	RIE::expand<u16, 4096>(biomesRIE, d->biomes);

	countRenderableBlocks();
}

void Chunk::compressIfIdle() {
	if (compressionState == CompressionState::COMPRESSED || difftime(time(0), lastUsed) < COMPRESS_DELAY) return;
	compress();
}

void Chunk::countRenderableBlocks() {
	renderableBlocks = 0;
	for (u16 i = 0; i < d->blocks.size(); i++)
		if (d->blocks[i] != DefinitionAtlas::AIR) renderableBlocks++;
}
