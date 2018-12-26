#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

#include "engine/Window.h"
#include "engine/Camera.h"

#include "engine/Timer.h"
#include "world/GameInstance.h"
#include "engine/graphics/Renderer.h"

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

int main(int argc, char* argv[]) {
    Timer boot("Initialization");

    auto renderer = new Renderer(1366, 768);
    auto game = new GameInstance();

    game->initialize(renderer);

    boot.printElapsedSeconds();

	//Game Loop
	while (!renderer->getWindow()->getShouldClose()) {
	    Timer t("Game Loop");

        auto now = (GLfloat)glfwGetTime();
        deltaTime = now - lastTime;
        lastTime = now;

        //Get & Handle Input
        glfwPollEvents();

        //Update game instance
        game->update(deltaTime);

        game->draw();

        float fps = 1000 / (t.elapsedNs() / 1000000.0f);
        std::ostringstream out;
        out.precision(2);
        out << std::fixed << fps;
        std::string s = out.str();

        game->fpsText->set(s + " FPS");
	}

	return 0;
}