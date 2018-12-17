#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "engine/Window.h"
#include "engine/Camera.h"

#include "engine/Timer.h"
#include "game/GameInstance.h"
#include "engine/graphics/Renderer.h"

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

//extern "C" {
//    #include "lua.h"
//    #include "lualib.h"
//    #include "lauxlib.h"
//}
//
//lua_State* L;
//
//static int average(lua_State *L) {
//    int n = lua_gettop(L);
//    double sum = 0;
//
//    for (int i = 1; i <= n; i++) {
//        sum += lua_tonumber(L, i);
//    }
//
//    lua_pushnumber(L, sum / n);
//    lua_pushnumber(L, sum);
//
//
//
//    return 2; //Number of results
//}

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

int main(int argc, char* argv[]) {
    Timer boot("Initialization");

    auto renderer = new Renderer(1366, 768);
    auto game = new GameInstance();

    game->initialize(renderer);

    boot.elapsed();

	//Game Loop
	while (!renderer->getWindow()->getShouldClose()) {
	    Timer t("Game Loop");

        auto now = (GLfloat)glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        //Get & Handle Input
        glfwPollEvents();
        renderer->update();

        //Update game instance
        game->update(deltaTime);

        renderer->begin();
        game->draw();
        renderer->end();

		t.elapsedMs(); //Print frame time
	}

	return 0;
}