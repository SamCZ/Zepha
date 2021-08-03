#include <iostream>

#include "ServerPacketStream.h"

#include "world/ServerWorld.h"
#include "util/net/Serializer.h"
#include "world/dim/chunk/Chunk.h"

ServerPacketStream::ServerPacketStream(ServerWorld& world) : world(world) {
	threads.reserve(THREADS);
	for (int i = 0; i < THREADS; i++) threads.emplace_back();
}

bool ServerPacketStream::queue(unsigned int dim, glm::ivec3 pos) {
	auto v4 = glm::ivec4(pos, dim);
	if (!queuedMap.count(v4)) {
//	if (queuedMap.find(v4) == queuedMap.end() && inProgressMap.find(v4) == inProgressMap.end()) {
		queuedTasks.push(v4);
		queuedMap.emplace(v4);
		return true;
	}
	return false;
}

ServerPacketStream::Thread::Thread() : thread(std::bind(&ServerPacketStream::Thread::run, this)) {}

std::unique_ptr<std::vector<std::unique_ptr<ServerPacketStream::FinishedJob>>> ServerPacketStream::update() {
	auto finished = std::make_unique<std::vector<std::unique_ptr<FinishedJob>>>();
	
	for (unsigned int i = 0; i < THREAD_QUEUE_SIZE; i++) {
		for (auto& t : threads) {
			auto& j = t.jobs[i];
			if (j.locked) continue;
			
			if (j.packet) {
				finished->emplace_back(make_unique<FinishedJob>(j.mapBlock->pos, j.dim, std::move(j.packet)));
				inProgressMap.erase(ivec4(j.mapBlock->pos, j.dim));
				j.mapBlock = nullptr;
			}
			
			if (!queuedTasks.empty()) {
				auto pos = queuedTasks.front();
				queuedMap.erase(pos);
				inProgressMap.emplace(pos);
				queuedTasks.pop();
				
//				std::cout << "going going" << std::endl;
				auto mapBlock = world.getDimension(pos.w)->getMapBlock(ivec3(pos));
//				std::cout << Util::toString(pos) << ": gone, " << mapBlock << std::endl;
				if (mapBlock == nullptr) continue;
//				std::cout << "mappi: " << Util::toString(j.mapBlock->pos) << std::endl;
				j.mapBlock = make_unique<MapBlock>(*mapBlock);
				j.dim = pos.w;
				j.locked = true;
			}
		}
	}
	
	return finished;
}

void ServerPacketStream::Thread::run() {
	while (!kill) {
		bool empty = true;
		for (Job& j : jobs) {
			if (j.locked) {
				empty = false;
				std::cout << "run: " << Util::toString(j.mapBlock->pos) << std::endl;
				
				Serializer s {};
				for (u16 i = 0; i < 64; i++) {
					auto chunk = j.mapBlock->get(i);
					if (chunk) s.append(chunk->compressToString());
				}
				
				j.packet = make_unique<Packet>(Packet::Type::MAPBLOCK);
				j.packet->data = s.data;
				j.locked = false;
			}
		}
		if (empty) std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}


ServerPacketStream::~ServerPacketStream() {
	for (auto& t : threads) {
		t.kill = true;
		t.thread.join();
	}
}