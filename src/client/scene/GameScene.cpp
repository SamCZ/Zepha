//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

#include "../lua/l_register_block.h"
#include "../lua/l_register_blockmodel.h"

GameScene::GameScene(ClientState* state) :
        Scene(state),
        gui(state),
        debugGui() {

    server = new ServerConnection("127.0.0.1", 12345);
    server->init();

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
    player->setPos(glm::vec3(0, 22, 0));

    debugGui.pushGuiObjects(guiEntities);
    gui.pushGuiObjects(guiEntities);

    auto crosshairTexture = new Texture((char*)"../res/tex/gui/crosshair.png");
    crosshairTexture->load();

    auto squareVerts = new std::vector<float> {
            -0.5, -0.5, 0, 0, 0, 0, 0, 0,
            -0.5,  0.5, 0, 0, 1, 0, 0, 0,
             0.5,  0.5, 0, 1, 1, 0, 0, 0,
             0.5, -0.5, 0, 1, 0, 0, 0, 0,

             0, -0.5, -0.5, 0, 0, 0, 0, 0,
             0,  0.5, -0.5, 0, 1, 0, 0, 0,
             0,  0.5,  0.5, 1, 1, 0, 0, 0,
             0, -0.5,  0.5, 1, 0, 0, 0, 0,

            -0.5, 0, -0.5, 0, 0, 0, 0, 0,
             0.5, 0, -0.5, 0, 1, 0, 0, 0,
             0.5, 0,  0.5, 1, 1, 0, 0, 0,
            -0.5, 0,  0.5, 1, 0, 0, 0, 0,
    };
    auto squareInds = new std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0,
            0, 2, 1, 2, 0, 3,
            4, 5, 6, 6, 7, 4,
            4, 6, 5, 6, 4, 7,
            8, 9, 10, 10, 11, 8,
            8, 10, 9, 10, 8, 11
    };

    auto m = new Mesh();
    m->create(squareVerts, squareInds);

    pointer = new Entity();
    pointer->create(m, crosshairTexture);

    delete squareVerts;
    delete squareInds;

    pointer->setPosition(glm::vec3(1, 18, 1));
    pointer->setScale(0.5);

    entities.push_back(pointer);
}


void GameScene::update() {
    server->update(*player, *pointer);

    auto window = state->renderer->getWindow();

    player->update(window->getKeysArray(), state->deltaTime, window->getDeltaX(), window->getDeltaY());

    debugGui.update(player, world, window, blockAtlas, state->fps);
    world->update();

//    pointer->setPosition(*player->getPos() + glm::vec3(0, -1, 0));
}

void GameScene::draw() {
    state->renderer->begin();
    state->renderer->enableWorldShader();

    textureAtlas->getTexture()->use();

    for (auto &chunk : *world->getMeshChunks())
        state->renderer->draw(chunk.second);

    Texture* prevTexture = nullptr;

    for (auto &entity : entities) {
        auto newTexture = entity->getTexture();

        if (newTexture != nullptr && newTexture != prevTexture) {
            prevTexture = newTexture;
            newTexture->use();
        }

        state->renderer->draw(entity);
    }

    state->renderer->enableGuiShader();

    for (auto &entity : guiEntities) {
        auto newTexture = entity->getTexture();

        if (newTexture != nullptr && newTexture != prevTexture) {
            prevTexture = newTexture;
            newTexture->use();
        }

        state->renderer->draw(entity);
    }

    state->renderer->end();
}

void GameScene::cleanup() {
    //TODO: Clean up
}