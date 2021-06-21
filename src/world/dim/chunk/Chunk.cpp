//
// Created by aurailus on 14/12/18.
//

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

Chunk::Chunk(glm::ivec3 pos, bool partial) :
	pos(pos),
	d(new ChunkData()),
	compressionState(CompressionState::DECOMPRESSED),
	generationState(partial ? GenerationState::PARTIAL : GenerationState::EMPTY) {}

Chunk::Chunk(const std::string& data) : c(data) {}

Chunk::~Chunk() {
	if (compressionState == CompressionState::DECOMPRESSED) delete d;
}

bool Chunk::setBlock(unsigned int ind, unsigned int blk) {
	assertDecompressed();
	if (ind > 4096) throw ChunkException(pos, "Index out of range.");
	
	if (d->blocks[ind] == blk) return false;
	d->blocks[ind] = blk;
	
	if (blk == DefinitionAtlas::AIR) renderableBlocks = std::max(renderableBlocks - 1, 0);
	else renderableBlocks++;
	
	return true;
}

const std::array<unsigned int, 4096>& Chunk::getBlocksArray() const {
	assertDecompressed();
	return d->blocks;
}

const std::array<unsigned short, 4096>& Chunk::getBiomesArray() const {
	assertDecompressed();
	return d->biomes;
}

void Chunk::combineWith(std::shared_ptr<Chunk> o) {
	for (unsigned int i = 0; i < 4096; i++)
		if (o->getBlock(i) > DefinitionAtlas::INVALID) setBlock(i, o->getBlock(i));
	
	if (generationState == GenerationState::GENERATED || o->isGenerated()) {
		generationState = GenerationState::GENERATED;
		countRenderableBlocks();
	}
	
	else generationState = GenerationState::PARTIAL;
}

std::string Chunk::compress() {
	Serializer s;
	
	std::vector<unsigned int> blocksRIE = {};
	std::vector<unsigned short> biomesRIE = {};
	
	RIE::encode<unsigned int, 4096>(d->blocks, blocksRIE);
	RIE::encode<unsigned short, 4096>(d->biomes, biomesRIE);
	
	std::string temp = Serializer().append(pos).appendVec(blocksRIE).appendVec(biomesRIE)
		.appendArr(d->blockLight).appendArr(d->sunLight).data;
	s.append<std::string>(gzip::compress(temp.data(), temp.size()));
	
	return s.data;
}

void Chunk::decompress(const std::string& data) {
	const auto& toDecompress = (data.length() ? data : c);
	std::string gzipped = Deserializer(toDecompress).read<std::string>();
	if (!gzip::is_compressed(gzipped.data(), gzipped.length()))
		throw std::runtime_error("Chunk contains invalid gzipped data.");
	
	c = "";
	d = new ChunkData {};
	compressionState = CompressionState::DECOMPRESSED;
	
	Deserializer(gzip::decompress(gzipped.data(), gzipped.length()))
		.read<glm::ivec3>(pos)
		.readArr<unsigned int, 4096>(d->blocks)
		.readArr<unsigned short, 4096>(d->biomes)
		.readArr<BlockLight, 4096>(d->blockLight)
		.readArr<SunLight, 2048>(d->sunLight);
	
	countRenderableBlocks();
}

void Chunk::countRenderableBlocks() {
	renderableBlocks = 0;
	for (int i = 0; i < d->blocks.size(); i++)
		if (d->blocks[i] != DefinitionAtlas::AIR) renderableBlocks++;
}