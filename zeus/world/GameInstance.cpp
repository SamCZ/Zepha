//
// Created by aurailus on 17/12/18.
//

#include "GameInstance.h"
#include "../lua_api/LRegisterBlock.h"

GameInstance::GameInstance() = default;

//TODO: Remove this function
void genChunks(World* world) {
    PerlinNoise p(0);

    int VIEW_RANGE = 24;

    for (int i = -VIEW_RANGE; i < VIEW_RANGE; i++) {
        for (int j = -VIEW_RANGE; j < VIEW_RANGE; j++) {
            for (int k = -1; k < 1; k++) {

                auto* blocks = new std::vector<int>();
                blocks->reserve(4096);

                glm::vec3 innerPos, pos;

                for (int ind = 0; ind < 4096; ind++) {
                    ArrayTrans3D::indAssignVec(ind, &innerPos);
                    pos.x = innerPos.x + i * CHUNK_SIZE;
                    pos.y = innerPos.y + k * CHUNK_SIZE;
                    pos.z = innerPos.z + j * CHUNK_SIZE;

                    double val = p.noise(pos.x / (double) 32, pos.z / (double) 32, pos.y / (double) 16) - pos.y * 0.08;

                    blocks->push_back((int)(val > 0.5));
                }

                world->newChunk(new glm::vec3(i, k, j), new BlockChunk(blocks));
            }
        }
    }
}


void GameInstance::initialize(Renderer* renderer) {
    this->renderer = renderer;

    textureAtlas = new TextureAtlas("../tex");
    blockAtlas = new BlockAtlas(textureAtlas);

    LuaParser p;
    p.init();
    auto Z = p.getModule();

    //Register APIs here
    LRegisterBlock(this).regApi(Z);

    p.doFile("../lua/file.lua");

    //Build the world
    //The world requires the blockAtlas for meshing and handling inputs.
    world = new World(blockAtlas);

    genChunks(world);
}

void GameInstance::update(GLfloat deltaTime) {
    auto camera = renderer->getCamera();
    auto window = renderer->getWindow();

    camera->keyControl(window->getKeysArray(), deltaTime);
    camera->mouseControl(window->getDeltaX(), window->getDeltaY());

    world->update();
}

void GameInstance::draw() {
    textureAtlas->getTexture()->use();

    for (auto &chunk : *world->getMeshChunks()) {
        renderer->draw(chunk.second);
    }

}

GameInstance::~GameInstance() = default;