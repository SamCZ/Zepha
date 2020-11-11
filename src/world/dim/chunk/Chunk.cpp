//
// Created by aurailus on 14/12/18.
//

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>

#include "Chunk.h"

#include "util/Util.h"
#include "util/net/Serializer.h"
#include "util/net/Deserializer.h"
#include "game/atlas/DefinitionAtlas.h"

Chunk::Chunk(const Chunk& o) :
	pos(o.pos), state(o.state),
	blocks(o.blocks), biomes(o.biomes),
	sunLight(o.sunLight), blockLight(o.blockLight),
	dirty(o.dirty), shouldRender(o.shouldRender),
	renderableBlocks(o.renderableBlocks) {}

Chunk::Chunk(glm::ivec3 pos, bool partial) : pos(pos), state(partial ? State::PARTIAL : State::EMPTY) {}

Chunk::Chunk(glm::ivec3 pos, const std::vector<unsigned int>& blocks, const std::vector<unsigned short>& biomes) :
	blocks(std::move(blocks)), biomes(std::move(biomes)),
	state(State::GENERATED), pos(pos) {
	countRenderableBlocks();
}

bool Chunk::setBlock(unsigned int ind, unsigned int blk) {
	if (!RIE::write(ind, blk, blocks, 4096)) return false;
	
	if (blk == DefinitionAtlas::AIR) {
		renderableBlocks = std::max(renderableBlocks - 1, 0);
		if (renderableBlocks == 0) shouldRender = false;
	}
	else {
		shouldRender = true;
		renderableBlocks++;
	}
	
	return true;
}

const std::vector<unsigned int>& Chunk::cGetBlocks() const {
	return blocks;
}

const std::vector<unsigned short>& Chunk::cGetBiomes() const {
	return biomes;
}

void Chunk::combineWith(std::shared_ptr<Chunk> o) {
	// TODO: Leverage the RIE streams to make this more efficient.
	
	for (unsigned int i = 0; i < 4096; i++) {
		if (o->getBlock(i) > DefinitionAtlas::INVALID) setBlock(i, o->getBlock(i));
	}
	
	if (state == State::GENERATED || o->isGenerated()) {
		state = State::GENERATED;
		countRenderableBlocks();
	}
	
	else state = State::PARTIAL;
}

std::string Chunk::serialize() {
	std::vector<unsigned short> blArray = std::vector<unsigned short>(4096);
	std::vector<unsigned char> slArray = std::vector<unsigned char>(2048);
	
	for (unsigned short i = 0; i < 4096; i++) {
		blocklight_union bl;
		bl.b = blockLight[i];
		blArray[i] = bl.sh;
	}
	
	for (unsigned short i = 0; i < 2048; i++) {
		sunlight_union sl;
		sl.s = sunLight[i];
		slArray[i] = sl.ch;
	}
	
	Serializer s;
	std::string temp = Serializer().append(pos).append(blocks).append(biomes).append(blArray).append(slArray).data;
	s.append<std::string>(gzip::compress(temp.data(), temp.size()));
	
	return s.data;
}

void Chunk::deserialize(Deserializer& d) {
	std::string gzipped = d.read<std::string>();
	if (!gzip::is_compressed(gzipped.data(), gzipped.length()))
		throw std::runtime_error("Chunk contains invalid gzipped data.");
	
	std::vector<unsigned char> slArray {};
	std::vector<unsigned short> blArray {};
	
	Deserializer(gzip::decompress(gzipped.data(), gzipped.length()))
		.read<glm::ivec3>(pos)
		.read<std::vector<unsigned int>>(blocks)
		.read<std::vector<unsigned short>>(biomes)
		.read<std::vector<unsigned short>>(blArray)
		.read<std::vector<unsigned char>>(slArray);
	
	for (unsigned short i = 0; i < 4096; i++) {
		blocklight_union bl;
		bl.sh = blArray[i];
		blockLight[i] = bl.b;
	}
	
	for (unsigned short i = 0; i < 2048; i++) {
		sunlight_union sl;
		sl.ch = slArray[i];
		sunLight[i] = sl.s;
	}
	
	countRenderableBlocks();
}

void Chunk::countRenderableBlocks() {
	shouldRender = false;
	renderableBlocks = 0;
	
	for (unsigned int i = 0; i < blocks.size(); i += 2) {
		unsigned int nInd = (i == blocks.size() - 2 ? 4095 : blocks[i + 2]);
		unsigned int cInd = blocks[i];
		
		if (blocks[i + 1] > DefinitionAtlas::AIR) {
			renderableBlocks += nInd - cInd;
			shouldRender = true;
		}
	}
}