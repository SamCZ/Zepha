//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

//TODO: Fix this
#include "../lua/l_register_block.h"
#include "../lua/l_register_blockmodel.h"

GameScene::GameScene(ClientState* state) : Scene(state) {
    server = new ServerConnection("127.0.0.1", 12345);
    auto info = server->connect();

    textureAtlas = new TextureAtlas("../res/tex");
    blockAtlas = new BlockAtlas(textureAtlas);

    LuaParser p;
    p.init();

    //Register APIs here
    l_register_block(this, &p);
    l_register_blockmodel(this, &p);

    p.doFile("../res/lua/file.lua");

    //The scene requires the blockAtlas for meshing and handling inputs.
    world = new World(blockAtlas);

    int SIZE = 12;
    for (int i = -SIZE; i < SIZE; i++) {
        for (int j = -12; j < 12; j++) {
            for (int k = -SIZE; k < SIZE; k++) {
                world->genNewChunk(glm::vec3(i, j, k));
            }
        }
    }

    player = new Player();
    player->create(world, state->renderer->getCamera());
    player->setPos(info->playerPos);

    world->genNewChunk(glm::vec3(0, 0, 0));

    gui.pushGuiObjects(guiEntities);

    auto crosshairTexture = new Texture((char*)"../res/tex/gui/crosshair.png");
    crosshairTexture->load();
    auto crosshair = new Entity();

    auto crossVerts = new std::vector<float> {
            -0.5, -0.5, 0, 0, 0, 0, 0, 0,
            -0.5,  0.5, 0, 0, 1, 0, 0, 0,
             0.5,  0.5, 0, 1, 1, 0, 0, 0,
             0.5, -0.5, 0, 1, 0, 0, 0, 0,
    };
    auto crossInds = new std::vector<unsigned int> {
        0, 1, 2, 2, 3, 0
    };

    auto m = new Mesh();
    m->create(crossVerts, crossInds);

    crosshair->create(m, crosshairTexture);

    delete crossVerts;
    delete crossInds;

    float xx = state->renderer->getWindow()->getBufferWidth()/2;
    float yy = state->renderer->getWindow()->getBufferHeight()/2;

    crosshair->setPosition(glm::vec3(xx, yy, 0));
    crosshair->setScale(22);

    guiEntities.push_back(crosshair);
}


void GameScene::update() {
    server->update();
    while (server->hasInPacket()) {
        auto packet = server->getPacket();

        if (packet->type == Packet::CHUNKINFO) world->loadChunkPacket(packet);

        if (packet->type != 4) {
            std::cout << "bad packet" << std::endl;
            return;
        }
//        delete packet;
    }

    auto window = state->renderer->getWindow();

    player->update(window->getKeysArray(), state->deltaTime, window->getDeltaX(), window->getDeltaY());

    gui.update(player, world, window, blockAtlas, state->fps);
    world->update();
}

void GameScene::draw() {
    state->renderer->begin();
    state->renderer->enableWorldShader();

    textureAtlas->getTexture()->use();

    for (auto &chunk : *world->getMeshChunks())
        state->renderer->draw(chunk.second);

    for (auto &entity : entities)
        state->renderer->draw(entity);

    state->renderer->enableGuiShader();

    Texture* prevTexture = nullptr;

    for (auto &gui : guiEntities) {
        if (gui->getTexture() != nullptr && gui->getTexture() != prevTexture) {
            prevTexture = gui->getTexture();
            gui->getTexture()->use();
        }
        state->renderer->drawGui(gui);
    }

    state->renderer->end();
}

void GameScene::cleanup() {
    //TODO: Clean up
}