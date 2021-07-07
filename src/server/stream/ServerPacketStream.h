	#pragma once

#include <queue>
#include <thread>
#include <glm/vec3.hpp>
#include <unordered_set>

#include "util/Vec.h"
#include "util/net/Packet.h"
#include "world/dim/chunk/MapBlock.h"

class ServerWorld;

class ServerPacketStream {
	public:
	static const int THREADS = 4;
	static const int THREAD_QUEUE_SIZE = 4;
	
	struct FinishedJob {
		FinishedJob(glm::ivec3 pos, unsigned int dim, std::unique_ptr<Packet> packet) :
			pos(pos), dim(dim), packet(std::move(packet)) {}
		
		glm::ivec3 pos;
		unsigned int dim;
		std::unique_ptr<Packet> packet;
	};
	
	ServerPacketStream(ServerWorld& world);
	
	bool queue(unsigned int dim, glm::ivec3 pos);
	
	std::unique_ptr<std::vector<std::unique_ptr<FinishedJob>>> update();
	
	~ServerPacketStream();
	
	private:
	struct Job {
		bool locked = false;
		unsigned int dim;
		std::unique_ptr<Packet> packet = nullptr;
		std::unique_ptr<MapBlock> mapBlock = nullptr;
	};
	
	struct Thread {
		Thread();
		
		void run();
		
		bool kill = false;
		
		std::vector<Job> jobs = std::vector<Job>(THREAD_QUEUE_SIZE);
		
		std::thread thread;
	};
	
	ServerWorld& world;
	
	std::vector<Thread> threads;
	std::queue<glm::ivec4> queuedTasks;
	std::unordered_set<glm::ivec4, Vec::ivec4> queuedMap;
	std::unordered_set<glm::ivec4, Vec::ivec4> inProgressMap;
};