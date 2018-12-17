#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "engine/Window.h"
#include "engine/Camera.h"
#include "engine/graphics/Shader.h"

#include "engine/TextureAtlas.h"
#include "blocks/BlockAtlas.h"
#include "world/World.h"

#include "engine/PerlinNoise.h"
#include "engine/helpers/ArrayTrans3D.h"
#include "engine/Timer.h"

Window* window;
Shader* shader;
Camera* camera;

TextureAtlas* textureAtlas;
BlockAtlas* blockAtlas;
World* world;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void registerBlocks() {
    auto* bm = BlockModel::Square("default_grass_top", "default_dirt", "default_grass_side", "default_grass_side", "default_grass_side", "default_grass_side", textureAtlas);
    auto* def = new BlockDef("grass", bm);
    blockAtlas->registerBlock(def);
}

void genChunks(World* world) {
    PerlinNoise p(0);

    int VIEW_RANGE = 32;

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

extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

lua_State* L;

static int average(lua_State *L) {
    int n = lua_gettop(L);
    double sum = 0;

    for (int i = 1; i <= n; i++) {
        sum += lua_tonumber(L, i);
    }

    lua_pushnumber(L, sum / n);
    lua_pushnumber(L, sum);



    return 2; //Number of results
}

int main(int argc, char* argv[]) {
//    int x = 4, y = 3, sum;
//
//    L = luaL_newstate();
//
//    //Open base libraries
//    luaL_openlibs(L);
//    lua_register(L, "average", average);
//
//    luaL_dofile(L, "../file.lua");
//
//    //Call the "add" function
//    lua_getglobal(L, "add");
//    lua_pushnumber(L, x);
//    lua_pushnumber(L, y);
//
//    lua_call(L, 2, 1);
//
//    sum = (int)lua_tointeger(L, -1);
//    lua_pop(L, 1);
//
//    lua_close(L);
//
//    printf("Hold up %i", sum);
//    getchar();
//
//    return 0;

	Timer boot("Initialization");

    window = new Window(1366, 768);
    window->initialize();

    camera = new Camera(glm::vec3(0.0f, 16.0f, 0.0f), glm::vec3(0, 1, 0), -90.0f, -45.0f, 10.0f, 0.1f);

	textureAtlas = new TextureAtlas("../tex");
    blockAtlas = new BlockAtlas(textureAtlas);
    registerBlocks();

	world = new World(blockAtlas);
    genChunks(world);

	shader = new Shader();
	shader->createFromFile("../src/shader/world.vs", "../src/shader/world.fs");

	glm::mat4 projectionMatrix = glm::perspective(45.0f, window->getBufferWidth() / window->getBufferHeight(), 0.1f, 1000.0f);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    boot.elapsed();

	//Game Loop
	while (!window->getShouldClose()) {
	    Timer t("Game Loop");

	    auto now = (GLfloat)glfwGetTime();
	    deltaTime = now - lastTime;
	    lastTime = now;

		//Get & Handle Input
		glfwPollEvents();
		window->update();

		camera->keyControl(window->getKeysArray(), deltaTime);
		camera->mouseControl(window->getDeltaX(), window->getDeltaY());

		world->update();

		//Clear Window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->useShader();
        textureAtlas->getTexture()->use();

        glUniformMatrix4fv(shader->getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(shader->getViewLocation(), 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

        //Render chunks
        world->draw(shader->getModelLocation());

        Shader::clearShader();
		window->swapBuffers();

//		t.elapsedMs(); //Print frame time
	}

	return 0;
}