//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"
#include "../lua_api/l_register_block.h"
#include "../lua_api/l_register_blockmodel.h"

GameScene::GameScene(ClientState* state) : Scene(state) {

    textureAtlas = new TextureAtlas("../tex");
    blockAtlas = new BlockAtlas(textureAtlas);

    LuaParser p;
    p.init();

    //Register APIs here
    l_register_block(this, &p);
    l_register_blockmodel(this, &p);

    p.doFile("../lua/file.lua");

    //The game requires the blockAtlas for meshing and handling inputs.
    world = new World(blockAtlas);

    int SIZE = 8;
    for (int i = -SIZE; i < SIZE; i++) {
        for (int j = 0; j < 8; j++) {
            for (int k = -SIZE; k < SIZE; k++) {
                world->genNewChunk(glm::vec3(i, j, k));
            }
        }
    }

    player = new Player();
    player->create(world, state->renderer->getCamera());
    player->setPos(glm::vec3(16, 24, 0));

    world->genNewChunk(glm::vec3(0, 0, 0));

    gui.pushGuiObjects(guiEntities);


    auto crosshairTexture = new Texture((char*)"../tex/gui/crosshair.png");
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

    auto camera = state->renderer->getCamera();
    auto window = state->renderer->getWindow();

    player->update(window->getKeysArray(), (GLfloat)state->deltaTime, window->getDeltaX(), window->getDeltaY());

    glm::vec3 round = World::roundVec(*camera->getPosition());
    round.y -= 2;

    int block = world->getBlock(round);
    std::string on = "Null";
    if (block >= 0) {
        on = blockAtlas->getBlock(block)->getIdentifier();
    }

    block = 0;
    for (Ray ray(player); ray.getLength() < 5; ray.step(0.01)) {
        auto found = world->getBlock(*ray.getEnd());
        if (found > 0) {
            block = found;
            if (state->renderer->getWindow()->mouseIsDown()) {
                world->setBlock(*ray.getEnd(), 0);
            }
            break;
        }
    }

    std::string look = "Null";
    if (block >= 0) {
        look = blockAtlas->getBlock(block)->getIdentifier();
    }

    gui.update(player->getPos(), player->getVel(), player->getYaw(), player->getPitch(), on, look, state->fps);

    world->update();
}

void GameScene::draw() {
    Timer t("Drawing");

    state->renderer->begin();

    state->renderer->enableWorldShader();
    textureAtlas->getTexture()->use();

    for (auto &chunk : *world->getMeshChunks()) {
        state->renderer->draw(chunk.second);
    }

    for (auto &entity : entities) {
        state->renderer->draw(entity);
    }

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