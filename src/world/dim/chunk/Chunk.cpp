#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>
#include <util/RIE.h>

#include "Chunk.h"

#include "util/Util.h"
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
	if (d != nullptr) *d = *o.d;
}

Chunk::Chunk(ivec3 pos, bool partial) :
	pos(pos),
	d(new ChunkData()),
	compressionState(CompressionState::DECOMPRESSED),
	generationState(partial ? GenerationState::PARTIAL : GenerationState::EMPTY) {}

Chunk::Chunk(const string& data) : c(data) {}

Chunk::~Chunk() {
	if (compressionState == CompressionState::DECOMPRESSED) delete d;
}

bool Chunk::setBlock(u16 ind, u16 blk) {
	assertDecompressed();
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	
	if (d->blocks[ind] == blk) return false;
	d->blocks[ind] = blk;
	
	if (blk == DefinitionAtlas::AIR) renderableBlocks = std::max(renderableBlocks - 1, 0);
	else renderableBlocks++;
	
	return true;
}

const std::array<u16, 4096>& Chunk::getBlocksArray() const {
	assertDecompressed();
	return d->blocks;
}

const std::array<u16, 4096>& Chunk::getBiomesArray() const {
	assertDecompressed();
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

string Chunk::compress() {
	Serializer s;
	
	std::vector<u16> blocksRIE = {};
	std::vector<u16> biomesRIE = {};
	
	RIE::encode<u16, 4096>(d->blocks, blocksRIE);
	RIE::encode<u16, 4096>(d->biomes, biomesRIE);
	
//	std::cout << "inr " << Util::vectorToString(blocksRIE) << std::endl;
//	std::cout << "in  " << Util::arrayToString(d->blocks) << std::endl;
	
	string temp = Serializer()
		.append(pos)
		.append(blocksRIE)
		.append(biomesRIE)
		.append(d->blockLight)
		.append(d->sunLight).data;
	
	s.append<string>(gzip::compress(temp.data(), temp.size()));
	
	return s.data;
}

void Chunk::decompress(const string& data) {
	const auto& toDecompress = data.length() ? data : c;
	string gzipped = Deserializer(toDecompress).read<string>();
	if (!gzip::is_compressed(gzipped.data(), gzipped.length()))
		throw ChunkException(pos, "Chunk contains invalid gzipped data.");
	
	c = "";
	d = new ChunkData {};
	compressionState = CompressionState::DECOMPRESSED;
	
	vec<u16> blocksRIE = {};
	vec<u16> biomesRIE = {};
	
	Deserializer(gzip::decompress(gzipped.data(), gzipped.length()))
		.read<ivec3>(pos)
		.read<vec<u16>>(blocksRIE)
		.read<vec<u16>>(biomesRIE)
		.read<array<BlockLight, 4096>>(d->blockLight)
		.read<array<SunLight, 2048>>(d->sunLight);
	
	RIE::expand<u16, 4096>(blocksRIE, d->blocks);
	RIE::expand<u16, 4096>(biomesRIE, d->biomes);
	
//	std::cout << "outr" << Util::vectorToString(blocksRIE) << std::endl;
//	std::cout << "out " << Util::arrayToString(d->blocks) << std::endl;
	
	countRenderableBlocks();
}

void Chunk::countRenderableBlocks() {
	renderableBlocks = 0;
	for (u16 i = 0; i < d->blocks.size(); i++)
		if (d->blocks[i] != DefinitionAtlas::AIR) renderableBlocks++;
}
