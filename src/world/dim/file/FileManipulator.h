//
// FileManipulator reads and writes world files.
// Created by aurailus on 2020-06-24.
//

#pragma once

#include <array>
#include <string>
#include <glm/vec3.hpp>
#include <cute_files.h>

class Chunk;

class FileManipulator {
	static constexpr unsigned int META_OFFSET = 12 + 2 * 4096;
	static constexpr unsigned int BLOCK_SIZE = 256;
	public:
	FileManipulator(const std::string& worldPath);
	
	void commitChunk(Chunk& chunk);
	
	private:
	static void writeChunk(std::fstream& file, unsigned int offset, const std::string& data);
	
	static void shiftChunks(std::fstream& file, std::array<short, 4096>& offsets, unsigned int ind, unsigned int amt);
	
	static void populateChunkOffsets(std::fstream& file, std::array<short, 4096>& offsets);
	
	void createRegionFileIfNotExists(glm::ivec3 pos);
	
	std::string path;
};
