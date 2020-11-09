//
// Created by aurailus on 2020-07-16.
//

#include <iostream>

#include "ServerPacketStream.h"

#include "world/ServerWorld.h"
#include "util/net/Serializer.h"
#include "world/dim/chunk/Chunk.h"

ServerPacketStream::ServerPacketStream(ServerWorld& world) {
	threads.reserve(THREADS);
	for (int i = 0; i < THREADS; i++) threads.emplace_back(world);
}

bool ServerPacketStream::queue(unsigned int dim, glm::ivec3 pos) {
	auto v4 = glm::ivec4(pos, dim);
	if (!queuedMap.count(v4)) {
		queuedTasks.push(v4);
		queuedMap.insert(v4);
		return true;
	}
	return false;
}

ServerPacketStream::Thread::Thread(ServerWorld& world) :
	world(world), thread(std::bind(&ServerPacketStream::Thread::run, this)) {}

std::unique_ptr<std::vector<std::unique_ptr<ServerPacketStream::FinishedJob>>> ServerPacketStream::update() {
	auto finished = std::make_unique<std::vector<std::unique_ptr<FinishedJob>>>();
	
	for (unsigned int i = 0; i < THREAD_QUEUE_SIZE; i++) {
		for (auto& t : threads) {
			auto& j = t.jobs[i];
			if (j.locked) continue;
			
			if (j.packet) finished->emplace_back(std::make_unique<FinishedJob>(j.pos, j.dim, std::move(j.packet)));
			
			if (!queuedTasks.empty()) {
				auto pos = queuedTasks.front();
				queuedMap.erase(pos);
				queuedTasks.pop();
				
				j.pos = glm::vec3(pos);
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
				
				auto mapBlock = world.getDimension(j.dim)->getMapBlock(j.pos);
				if (!mapBlock) {
					j.locked = false;
					continue;
				}
				
				Serializer s{};
				for (unsigned int i = 0; i < 64; i++) {
					auto chunk = mapBlock->get(i);
					if (chunk) s.append(chunk->serialize());
				}
				
				j.packet = std::make_unique<Packet>(Packet::Type::MAPBLOCK);
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