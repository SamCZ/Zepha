//
// Created by aurailus on 16/03/19.
//

#include "WorldInterpolationStream.h"

#include "world/LocalWorld.h"
#include "util/net/PacketView.h"
#include "world/dim/chunk/Chunk.h"

WorldInterpolationStream::WorldInterpolationStream(LocalSubgame& game, LocalWorld& world, unsigned int seed) {
	threads.reserve(THREADS);
	for (int i = 0; i < THREADS; i++) threads.emplace_back(game, world, seed);
}

void WorldInterpolationStream::queuePacket(std::unique_ptr<PacketView> p) {
	queuedPacketTasks.push(std::move(p));
}

//bool WorldInterpolationStream::queuePosition(glm::vec3 pos){
//    if (!queuedInterpMap.count(pos)) {
//        queuedInterpTasks.push_back(pos);
//        queuedInterpMap.insert(pos);
//        return true;
//    }
//    return false;
//}

std::unique_ptr<std::vector<std::shared_ptr<Chunk>>> WorldInterpolationStream::update() {
	auto finishedChunks = std::make_unique<std::vector<std::shared_ptr<Chunk>>>();
//    auto finishedMapBlocks = std::make_unique<std::vector<std::shared_ptr<MeshFarMap>>>();
	
	for (unsigned int i = 0; i < THREAD_QUEUE_SIZE; i++) {
		for (auto& t : threads) {
			auto& j = t.jobs[i];
			if (j.locked) continue;
			
			if (!j.chunks.empty()) {
				for (auto& c : j.chunks) finishedChunks->emplace_back(std::move(c));
				j.chunks.clear();
//                j.job = JobType::EMPTY;
			}
//            else if (u.mapblock != nullptr) {
//                finishedMapBlocks->push_back(u.mapblock);
//                u.mapblock = nullptr;
//                u.job = JobType::EMPTY;
//            }
			
			if (!queuedPacketTasks.empty()) {
				auto packet = std::move(queuedPacketTasks.front());
				queuedPacketTasks.pop();

//                j.job = JobType::PACKET;
				j.packet = std::move(packet);
				j.locked = true;
				
			}
//            else if (!queuedInterpTasks.empty()) {
//                auto it = queuedInterpTasks.begin();
//                glm::vec3 pos = *it;
//                queuedInterpTasks.erase(it);
//                queuedInterpMap.erase(pos);
//
//                j.job = JobType::FARMAP;
//                j.mapBlockPos = pos;
//                j.locked = true;
//            }
		}
	}
	
	return finishedChunks;
}

WorldInterpolationStream::Thread::Thread(LocalSubgame& game, LocalWorld& world, unsigned int seed) :
	thread(std::bind(&WorldInterpolationStream::Thread::run, this)) {}

void WorldInterpolationStream::Thread::run() {
	while (!kill) {
		bool empty = true;
		for (Job& u : jobs) {
			if (u.locked) {
//                if (u.job == JobType::PACKET) {
				empty = false;
				
				if (u.packet->type == Packet::Type::CHUNK) {
					u.chunks.reserve(1);
					u.chunks.emplace_back(std::make_shared<Chunk>());
					u.chunks.back()->deserialize(u.packet->d);
				}
				else if (u.packet->type == Packet::Type::MAPBLOCK) {
					u.chunks.reserve(64);
					while (!u.packet->d.atEnd()) {
						std::string dat = u.packet->d.read<std::string>();
						Deserializer d(dat);
						u.chunks.emplace_back(std::make_shared<Chunk>());
						u.chunks.back()->deserialize(d);
					}
				}
				
				u.locked = false;
//                }
//                else if (u.job == JobType::FARMAP) {
//                    throw std::runtime_error("Farmap no exist yet.");
//                }
			}
		}
		if (empty) std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
}

WorldInterpolationStream::~WorldInterpolationStream() {
	for (auto& t : threads) {
		t.kill = true;
		t.thread.join();
	}
}