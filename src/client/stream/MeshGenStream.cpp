//
// Created by aurailus on 27/03/19.
//

//#include <libnoise/noise.h>

#include "MeshGenStream.h"

#include "MeshChunkDetails.h"
#include "client/graph/mesh/ChunkMeshGenerator.h"
#include "world/dim/chunk/Chunk.h"
#include "world/dim/LocalDimension.h"

MeshGenStream::MeshGenStream(SubgamePtr game, LocalDimension& dimension): dimension(dimension),
	noiseSampler({ NoiseSample(u16vec3(16), 1), NoiseSample(u16vec3(16), 1), NoiseSample(u16vec3(16), 1) }) {
	let simplex = FastNoise::New<FastNoise::Simplex>();
	let generator = FastNoise::New<FastNoise::DomainScale>();
	generator->SetSource(simplex);
	generator->SetScale(1000.f);
	
	// 8 is just a random value to offset results
	noiseSampler[0].generate(u16vec3(8, 0, 0), generator);
	noiseSampler[1].generate(u16vec3(0, 8, 0), generator);
	noiseSampler[2].generate(u16vec3(0, 0, 8), generator);
	
	threads.reserve(THREADS);
	for (int i = 0; i < THREADS; i++) threads.emplace_back(*game.l(), noiseSampler);
}

std::vector<MeshChunkDetails*> MeshGenStream::update() {
	std::vector<MeshChunkDetails*> finishedChunks;
	
	for (u16 i = 0; i < THREAD_QUEUE_SIZE; i++) {
		for (Thread& t : threads) {
			auto& j = t.jobs[i];
			if (j.busy) continue;
			
			if (j.meshDetails->vertices.size()) {
				j.thisChunk = nullptr;
				finishedChunks.push_back(j.meshDetails);
				j.meshDetails = new MeshChunkDetails();
			}
			
			if (!queuedTasks.empty()) {
				auto pos = queuedTasks.front();
				queuedMap.erase(pos);
				queuedTasks.pop_front();
				
				std::shared_ptr<Chunk> chunk = dimension.getChunk(pos);
				if (!chunk) goto breakAddTask;
				
				
				j.meshDetails->pos = pos;
				j.detail = glm::distance(vec3 {}, vec3(pos)) <= HIGH_DETAIL_RANGE ?
					ChunkMeshGenerator::Detail::HIGH : ChunkMeshGenerator::Detail::LOW;
				j.thisChunk = make_unique<Chunk>(*chunk);
				
				int ind = 0;
				for (const glm::ivec3& dir : Vec::TO_VEC) {
					std::shared_ptr<Chunk> adjacent = dimension.getChunk(pos + dir);
					if (!adjacent) goto breakAddTask;
					j.adjacentChunks[ind++] = std::make_unique<Chunk>(*adjacent);
				}
				
				j.busy = true;
				
			}
			
			breakAddTask:;
		}
	}
	
	return std::move(finishedChunks);
}

MeshGenStream::Thread::Thread(LocalSubgame& defs, std::array<NoiseSample, 3>& offsetSamplers) :
	game(defs), offsetSamplers(offsetSamplers), thread(std::bind(&MeshGenStream::Thread::exec, this)) {}

void MeshGenStream::Thread::exec() {
	while (keepAlive) {
		bool empty = true;
		
		for (auto i = 0; i < jobs.size(); i++) {
			auto& u = jobs[i];
			if (!u.busy) continue;
			
			assert(u.thisChunk);
			for (int i = 0; i < u.adjacentChunks.size(); i++) assert(u.adjacentChunks[i]);
			ChunkMeshGenerator m(u.meshDetails, game.getDefs(), game.getBiomes(),
				std::move(u.thisChunk), std::move(u.adjacentChunks), offsetSamplers, ChunkMeshGenerator::Detail::HIGH);
			empty = false;
			u.busy = false;
		}
		
		if (empty) std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

void MeshGenStream::queue(glm::ivec3 pos, bool priority) {
	if (!queuedMap.count(pos)) {
		if (priority) queuedTasks.push_front(pos);
		else queuedTasks.push_back(pos);
		queuedMap.insert(pos);
	}
}

MeshGenStream::~MeshGenStream() {
	for (auto& t : threads) {
		t.keepAlive = false;
		t.thread.join();
	}
}