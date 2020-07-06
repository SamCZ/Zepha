//
// Created by aurailus on 2020-06-24.
//
// File Format:
// 12 bytes -- Region Location Vec3
// 2 * 4096 bytes - Chunk offset within the file. -1 means it doesn't exist
// 1024 byte subsections for containing Chunks.
//
// Chunk Format:
// 4 bytes - Data length
// [remaining] - Serialized Chunk data
//

#include <fstream>
#include <iostream>

#include "FileManipulator.h"

#include "../chunk/Chunk.h"
#include "../../util/Util.h"
#include "../../util/net/Serializer.h"
#include "../../util/net/Deserializer.h"

FileManipulator::FileManipulator(const std::string &worldPath) :
    path(worldPath) {
}

void FileManipulator::commitChunk(Chunk &chunk) {
//    return;
    glm::ivec3 reg = Space::Region::world::fromChunk(chunk.pos);
    unsigned int chunkInd = Space::Chunk::index(chunk.pos);

    std::string fileName = std::to_string(reg.x) + "_" + std::to_string(reg.y) + "_" + std::to_string(reg.z);
    std::string filePath = path + "/" + fileName;
    createRegionFileIfNotExists(reg);

    std::string chunkData = chunk.serialize();
    unsigned int dataBlockSize = floor(chunkData.length() / BLOCK_SIZE);

    std::fstream file(filePath, std::ios::in|std::ios::out|std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Couldn't open file.");

    std::array<short, 4096> chunkOffsets {};
    populateChunkOffsets(file, chunkOffsets);

    unsigned int currentBlock = chunkOffsets[chunkInd];
    unsigned int nextBlock = (chunkInd == 63 ? 10000 : chunkOffsets[chunkInd + 1]);

    if (dataBlockSize > nextBlock - currentBlock)
        shiftChunks(file, chunkOffsets, chunkInd + 1, dataBlockSize - (nextBlock - currentBlock));
    writeChunk(file, chunkOffsets[chunkInd], chunkData);

    file.close();
}

void FileManipulator::writeChunk(std::fstream& file, unsigned int offset, const std::string& data) {
    file.seekp(META_OFFSET + offset * BLOCK_SIZE);
    file.write(data.data(), data.length());
}

void FileManipulator::shiftChunks(std::fstream &file, std::array<short, 4096>& offsets, unsigned int ind, unsigned int amt) {
    std::cout << "Shifting by " << amt << std::endl;

    unsigned int startPos = offsets[ind] * BLOCK_SIZE + META_OFFSET;
    unsigned int endPos; {
        char *buff = new char[4];
        file.seekg(META_OFFSET + offsets[63] * BLOCK_SIZE);
        file.read(buff, 4);
        endPos = META_OFFSET + BLOCK_SIZE * offsets[63] + Deserializer(buff, 4).read<unsigned int>();
    }

    unsigned int len = endPos - startPos;
    char* buff = new char[len];
    file.seekg(offsets[ind]);
    file.read(buff, len);
    file.seekp(offsets[ind] + amt * BLOCK_SIZE);
    file.write(buff, len);
    delete[] buff;

    for (unsigned int i = ind; i < 4096; i++) {
        offsets[i] += amt;
        Serializer s;
        s.append<short>(offsets[i]);
        file.seekp(12 + i * 2);
        file.write(s.data.data(), s.data.length());
        file.close();
    }

    std::cout << "Shift done" << std::endl;
}


void FileManipulator::populateChunkOffsets(std::fstream& file, std::array<short, 4096>& offsets) {
    file.seekg(12);
    char* buff = new char[2];
    for (unsigned int i = 0; i < 4096; i++) {
        file.read(buff, 2);
        offsets[i] = Deserializer(buff, 2).read<short>();
    }
    delete[] buff;
}

void FileManipulator::createRegionFileIfNotExists(glm::ivec3 pos) {
    std::string fileName = std::to_string(pos.x) + "_" + std::to_string(pos.y) + "_" + std::to_string(pos.z);
    std::string filePath = path + fileName;

    if (cf_file_exists(filePath.data())) return;

    std::fstream file(filePath, std::ios::out|std::ios::binary);
    if (!file.is_open()) throw std::runtime_error("Couldn't open file.");
    file.seekp(0);

    Serializer s;
    s.append(pos);
    for (int i = 0; i < 4096; i++) s.append<short>(i);
    for (int i = 0; i < 4096 * BLOCK_SIZE; i++) s.append<char>(0);

    file.write(s.data.data(), s.data.length());
    file.close();
}