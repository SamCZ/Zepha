//
// Created by aurailus on 06/03/19.
//

#pragma once

#include <queue>
#include <thread>
#include <shared_mutex>
#include <unordered_set>

#include "util/Types.h"
#include "world/gen/MapGen.h"
#include "util/CovariantPtr.h"

class ServerSubgame;

class ServerGenStream {
public:
	static const usize THREADS = 6;
	static const usize THREAD_QUEUE_SIZE = 6;
	
	struct FinishedJob {
		FinishedJob(u16 dim, ivec3 pos, uptr<MapGen::ChunkMap> created) :
			dim(dim), pos(pos), created(std::move(created)) {};
		
		u16 dim;
		ivec3 pos;
		uptr<MapGen::ChunkMap> created;
	};
	
	explicit ServerGenStream(ServerSubgame& game, ServerWorld& world);
	
	~ServerGenStream();
	
	bool queue(u16 dim, ivec3 pos);
	
	std::unique_ptr<std::vector<FinishedJob>> update();
	
	private:
	struct Job {
		bool locked = false;
		uptr<MapGen::ChunkMap> created = nullptr;
		
		u16 dim;
		ivec3 pos{};
		sptr<MapGen> gen;
	};
	
	struct Thread {
		explicit Thread(ServerSubgame& game, ServerWorld& world);
		
		void run();
		
		bool kill = false;
		
		vec<Job> jobs = vec<Job>(THREAD_QUEUE_SIZE);
		
		std::thread thread;
	};
	
	ServerWorld& world;
	vec<Thread> threads;
	std::queue<ivec4> queuedTasks;
	std::unordered_set<ivec4, Vec::ivec4> queuedMap;
	std::unordered_set<ivec4, Vec::ivec4> inProgressMap;
};