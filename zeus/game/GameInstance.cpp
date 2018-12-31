//
// Created by aurailus on 17/12/18.
//

#include "GameInstance.h"
#include "../lua_api/LRegisterBlock.h"
#include "../engine/Ray.h"

GameInstance::GameInstance() = default;

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
    player->create(world, renderer->getCamera());
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

    crosshair->setPosition(glm::vec3(renderer->getWindow()->getBufferWidth()/2, renderer->getWindow()->getBufferHeight()/2, 0));
    crosshair->setScale(22);

    guiEntities.push_back(crosshair);
}

void GameInstance::update(GLfloat deltaTime, double fps) {
    renderer->update();

    auto camera = renderer->getCamera();
    auto window = renderer->getWindow();

    player->update(window->getKeysArray(), deltaTime, window->getDeltaX(), window->getDeltaY());

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
            if (renderer->getWindow()->mouseIsDown()) {
                world->setBlock(*ray.getEnd(), 0);
            }
            break;
        }
    }

    std::string look = "Null";
    if (block >= 0) {
        look = blockAtlas->getBlock(block)->getIdentifier();
    }

    gui.update(player->getPos(), player->getVel(), player->getYaw(), player->getPitch(), on, look, fps);

    world->update();
}

void GameInstance::draw() {
    Timer t("Drawing");

    renderer->begin();

    renderer->enableWorldShader();
    textureAtlas->getTexture()->use();

    for (auto &chunk : *world->getMeshChunks()) {
        renderer->draw(chunk.second);
    }

    for (auto &entity : entities) {
        renderer->draw(entity);
    }

    renderer->enableGuiShader();

    Texture* prevTexture = nullptr;

    for (auto &gui : guiEntities) {
        if (gui->getTexture() != nullptr && gui->getTexture() != prevTexture) {
            prevTexture = gui->getTexture();
            gui->getTexture()->use();
        }

        renderer->drawGui(gui);
    }

    renderer->end();
}
