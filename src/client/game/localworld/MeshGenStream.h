//
// Created by aurailus on 27/03/19.
//

#ifndef ZEUS_MESHGENSTREAM_H
#define ZEUS_MESHGENSTREAM_H


#include <vec3.hpp>
#include <thread>
#include <unordered_set>
#include "../../../generic/blocks/BlockChunk.h"
#include "../../../generic/helpers/Vec3Compare.h"
#include "../../../generic/blocks/BlockAtlas.h"
#include "../../graphics/mesh/MeshGenerator.h"

class MeshGenStream {
public:
    static const int THREAD_QUEUE_SIZE = 32;
    static const int THREADS = 4;
    static const int TOTAL_QUEUE_SIZE = THREADS * THREAD_QUEUE_SIZE;

    MeshGenStream();
    explicit MeshGenStream(BlockAtlas* atlas);
    ~MeshGenStream();

    bool spaceInQueue();
    bool isQueued(glm::vec3 pos);
    //Attempt to add `pos` to the pre-thread queue.
    //Will return a boolean stating if there is more space left in the queue.
    bool tryToQueue(std::pair<BlockChunk*, std::vector<bool>*> data);

    struct MeshDetails {
        std::vector<float>* vertices;
        std::vector<unsigned int>* indices;
        glm::vec3 pos {0, 0, 0};

        MeshDetails(std::vector<float>* verts, std::vector<unsigned int>* inds, glm::vec3 p) {
            this->vertices = verts;
            this->indices = inds;
            this->pos = p;
        }
    };

    //Will return a vector of MeshDetails pointers containing finished meshes.
    //Frees up the threads and starts new tasks.
    std::vector<MeshDetails>* update();

    struct Unit {
        BlockChunk* chunk = nullptr;
        std::vector<bool>* adjacent = nullptr;

        std::vector<float>* vertices = nullptr;
        std::vector<unsigned int>* indices = nullptr;

        bool unlocked = true;
    };

    struct Thread {
        explicit Thread(BlockAtlas* atlas);

        BlockAtlas* atlas;

        std::thread* thread;
        bool keepAlive = true;

        std::vector<Unit> tasks = std::vector<Unit>(THREAD_QUEUE_SIZE);
    };

    std::vector<Thread> threads;
private:
    static void threadFunction(Thread* thread);

    BlockAtlas* atlas;

    std::vector<std::pair<BlockChunk*, std::vector<bool>*>> queuedTasks;
    std::unordered_set<glm::vec3, Vec3Compare::func> queuedMap;
};


#endif //ZEUS_MESHGENSTREAM_H
