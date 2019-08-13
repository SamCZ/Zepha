//
// Created by aurailus on 27/03/19.
//

#ifndef ZEUS_MESHGENSTREAM_H
#define ZEUS_MESHGENSTREAM_H


#include <glm/vec3.hpp>
#include <thread>
#include <unordered_set>
#include "../../../world/chunk/BlockChunk.h"
#include "../../../def/LocalDefinitionAtlas.h"
#include "graph/MeshGenerator.h"
#include "../../../util/Vec.h"
#include "../../../world/Dimension.h"
#include "../../../def/LocalDefs.h"
#include "MeshDetails.h"

class MeshGenStream {
public:
    static const int THREAD_QUEUE_SIZE = 32;
    static const int THREADS = 4;
    static const int TOTAL_QUEUE_SIZE = THREADS * THREAD_QUEUE_SIZE;

    explicit MeshGenStream(LocalDefs& defs, Dimension& dimension);
    ~MeshGenStream();

    bool spaceInQueue();
    bool isQueued(glm::vec3 pos);
    //Attempt to add `pos` to the pre-thread queue.
    //Will return a boolean stating if there is more space left in the queue.
    bool tryToQueue(glm::vec3 pos);

    //Will return a vector of MeshDetails pointers containing finished meshes.
    //Frees up the threads and starts new tasks.
    std::vector<MeshDetails*> update();

    struct Unit {
        std::shared_ptr<BlockChunk> thisChunk = nullptr;
        std::array<std::shared_ptr<BlockChunk>, 6> adjacentChunks {};

        MeshDetails* meshDetails = new MeshDetails();

        bool busy = false;
    };

    struct Thread {
        explicit Thread(LocalDefinitionAtlas &atlas, std::array<NoiseSample, 3>& offsetSampler);

        LocalDefinitionAtlas &atlas;
        std::array<NoiseSample, 3>& offsetSamplers;

        std::thread thread {};
        bool keepAlive = true;

        std::vector<Unit> tasks = std::vector<Unit>(THREAD_QUEUE_SIZE);
    };

    std::vector<Thread> threads;
private:
    static void threadFunction(Thread* thread);

    Dimension& dimension;
    LocalDefs& defs;

    std::array<NoiseSample, 3> noiseSampler;
    std::vector<glm::vec3> queuedTasks;
    std::unordered_set<glm::vec3, VecUtils::compareFunc> queuedMap;
};


#endif //ZEUS_MESHGENSTREAM_H
