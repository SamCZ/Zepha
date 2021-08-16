//
// Created by aurailus on 06/03/19.
//

#include "ServerGenStream.h"

#include "world/ServerWorld.h"
#include "game/ServerSubgame.h"
#include "world/dim/chunk/Chunk.h"

ServerGenStream::ServerGenStream(ServerSubgame& game, ServerWorld& world) :
	world(world) {
	threads.reserve(THREADS);
	for (usize i = 0; i < THREADS; i++) threads.emplace_back(game, world);
}

bool ServerGenStream::queue(u16 dimension, ivec3 pos) {
	auto v4 = ivec4(pos, dimension);
	if (queuedMap.find(v4) == queuedMap.end() && inProgressMap.find(v4) == inProgressMap.end()) {
		queuedTasks.push(v4);
		queuedMap.emplace(v4);
		return true;
	}
	return false;
}

uptr<vec<ServerGenStream::FinishedJob>> ServerGenStream::update() {
	auto created = make_unique<vec<FinishedJob>>();
	
	usize dequeued = 0;
	for (usize i = 0; i < THREAD_QUEUE_SIZE; i++) {
		for (auto& t : threads) {
			auto& j = t.jobs[i];
			if (j.locked) continue;
			
			if (j.created) {
				inProgressMap.erase(ivec4(j.pos, j.dim));
				created->emplace_back(j.dim, j.pos, std::move(j.created));
			}
			
			if (!queuedTasks.empty()) {
				auto pos = queuedTasks.front();
				queuedMap.erase(pos);
				inProgressMap.emplace(pos);
				queuedTasks.pop();
				dequeued++;
				
				j.pos = ivec3(pos);
				j.gen = world.getDimension(pos.w)->getGen();
				j.dim = pos.w;
				j.locked = true;
			}
		}
	}
	
	if (dequeued >= THREAD_QUEUE_SIZE * THREADS)
		std::cout << Log::err << "Server threads were idle!" << Log::endl;
	
	return created;
}

ServerGenStream::Thread::Thread(ServerSubgame& game, ServerWorld& world) :
	thread(std::bind(&ServerGenStream::Thread::run, this)) {}

void ServerGenStream::Thread::run() {
	while (!kill) {
		bool empty = true;
		for (Job& j : jobs) {
			if (j.locked) {
				empty = false;
				j.created = j.gen->generateMapBlock(j.dim, j.pos);
				j.locked = false;
			}
		}
		if (empty) std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

ServerGenStream::~ServerGenStream() {
	for (auto& t : threads) {
		t.kill = true;
		t.thread.join();
	}
}