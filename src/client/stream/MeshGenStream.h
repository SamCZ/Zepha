//
// Created by aurailus on 27/03/19.
//

#pragma once

#include <queue>
#include <thread>
#include <glm/vec3.hpp>
#include <unordered_set>

#include "util/Vec.h"
#include "util/CovariantPtr.h"
#include "MeshChunkDetails.h"
#include "world/gen/NoiseSample.h"

class Chunk;

class LocalSubgame;

class LocalDimension;

class MeshGenStream {
public:
	static const u16 THREADS = 6;
	static const u16 THREAD_QUEUE_SIZE = 12;
	
	explicit MeshGenStream(SubgamePtr game, LocalDimension& dimension);
	
	~MeshGenStream();
	
	void queue(glm::ivec3 pos, bool priority = false);
	
	//Will return a vector of MeshDetails pointers containing finished meshes.
	//Frees up the threads and starts new tasks.
	std::vector<MeshChunkDetails*> update();
	
	struct Job {
		std::unique_ptr<Chunk> thisChunk = nullptr;
		std::array<std::unique_ptr<Chunk>, 6> adjacentChunks {};
		
		MeshChunkDetails* meshDetails = new MeshChunkDetails();
		
		bool busy = false;
	};
	
	struct Thread {
		explicit Thread(LocalSubgame& defs, std::array<NoiseSample, 3>& offsetSampler);
		
		void exec();
		
		LocalSubgame& game;
		std::array<NoiseSample, 3>& offsetSamplers;
		std::vector<Job> jobs = std::vector<Job>(THREAD_QUEUE_SIZE);
		
		bool keepAlive = true;
		std::thread thread{};
	};
	
	std::vector<Thread> threads;

private:
	
	LocalDimension& dimension;
	
	std::array<NoiseSample, 3> noiseSampler;
	std::deque<glm::ivec3> queuedTasks;
	std::unordered_set<glm::vec3, Vec::vec3> queuedMap;
};

