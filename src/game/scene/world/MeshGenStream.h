//
// Created by aurailus on 27/03/19.
//

#ifndef ZEUS_MESHGENSTREAM_H
#define ZEUS_MESHGENSTREAM_H


#include <vec3.hpp>
#include <thread>
#include <unordered_set>
#include "../../../world/chunk/BlockChunk.h"
#include "../../../def/block/BlockAtlas.h"
#include "graph/MeshGenerator.h"
#include "../../../util/Vec.h"
#include "../../../world/Dimension.h"
#include "../../../def/GameDefs.h"

class MeshGenStream {
public:
    static const int THREAD_QUEUE_SIZE = 32;
    static const int THREADS = 4;
    static const int TOTAL_QUEUE_SIZE = THREADS * THREAD_QUEUE_SIZE;

    explicit MeshGenStream(GameDefs& defs, Dimension& dimension);
    ~MeshGenStream();

    bool spaceInQueue();
    bool isQueued(glm::vec3 pos);
    //Attempt to add `pos` to the pre-thread queue.
    //Will return a boolean stating if there is more space left in the queue.
    bool tryToQueue(glm::vec3 pos);

    struct MeshDetails {
        std::vector<Vertex>* vertices;
        std::vector<unsigned int>* indices;
        glm::vec3 pos {0, 0, 0};

        MeshDetails(std::vector<Vertex>* verts, std::vector<unsigned int>* inds, glm::vec3 p) {
            this->vertices = verts;
            this->indices = inds;
            this->pos = p;
        }
    };

    //Will return a vector of MeshDetails pointers containing finished meshes.
    //Frees up the threads and starts new tasks.
    std::vector<MeshDetails> update();

    struct Unit {
        std::shared_ptr<BlockChunk> chunk = nullptr;
        std::vector<bool>* adjacent = nullptr;

        std::vector<Vertex>* vertices = nullptr;
        std::vector<unsigned int>* indices = nullptr;

        bool unlocked = true;
    };

    struct Thread {
        explicit Thread(BlockAtlas &atlas);

        BlockAtlas &atlas;

        std::thread* thread;
        bool keepAlive = true;

        std::vector<Unit> tasks = std::vector<Unit>(THREAD_QUEUE_SIZE);
    };

    std::vector<Thread> threads;
private:
    std::vector<bool>* getAdjacentsCull(glm::vec3 pos);
    static void threadFunction(Thread* thread);

    Dimension& dimension;
    GameDefs& defs;

    std::vector<glm::vec3> queuedTasks;
    std::unordered_set<glm::vec3, VecUtils::compareFunc> queuedMap;
};


#endif //ZEUS_MESHGENSTREAM_H
