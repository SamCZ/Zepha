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

BlockAtlas* createAtlas() {
    Vertex* leftVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 0.0f, 0.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(0.0f, 0.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 1.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
    };
    auto* leftInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* leftPart = new MeshPart(leftVerts, 4, leftInds, 6, "default_grass_side", textureAtlas);

    Vertex* rightVerts = new Vertex[4] {
            Vertex(new glm::vec3(1.0f, 0.0f, 0.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 1.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
    };
    auto* rightInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* rightPart = new MeshPart(rightVerts, 4, rightInds, 6, "default_grass_side", textureAtlas);

    Vertex* topVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 1.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 1.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 0.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
    };
    auto* topInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* topPart = new MeshPart(topVerts, 4, topInds, 6, "default_grass_top", textureAtlas);

    Vertex* bottomVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 0.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 0.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
            Vertex(new glm::vec3(0.0f, 0.0f, 1.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
    };
    auto* bottomInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* bottomPart = new MeshPart(bottomVerts, 4, bottomInds, 6, "default_dirt", textureAtlas);

    Vertex* frontVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 0.0f, 1.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 1.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 1.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 1.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
    };
    auto* frontInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* frontPart = new MeshPart(frontVerts, 4, frontInds, 6, "default_grass_side", textureAtlas);

    Vertex* backVerts = new Vertex[4] {
            Vertex(new glm::vec3(0.0f, 0.0f, 0.0f), nullptr, new glm::vec2(0.0f, 1.0f)),
            Vertex(new glm::vec3(0.0f, 1.0f, 0.0f), nullptr, new glm::vec2(0.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 1.0f, 0.0f), nullptr, new glm::vec2(1.0f, 0.0f)),
            Vertex(new glm::vec3(1.0f, 0.0f, 0.0f), nullptr, new glm::vec2(1.0f, 1.0f)),
    };
    auto* backInds = new unsigned int[6] {
            0, 1, 2, 2, 3, 0
    };

    auto* backPart = new MeshPart(backVerts, 4, backInds, 6, "default_grass_side", textureAtlas);

    auto* bm = new BlockModel(leftPart, rightPart, topPart, bottomPart, frontPart, backPart, nullptr, true, true);
    return new BlockAtlas(bm);
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

int main(int argc, char* argv[]) {
	Timer boot("Initialization");

    window = new Window(1366, 768);
    window->initialize();

    camera = new Camera(glm::vec3(0.0f, 16.0f, 0.0f), glm::vec3(0, 1, 0), -90.0f, -45.0f, 10.0f, 0.1f);

	textureAtlas = new TextureAtlas("../Textures");
    blockAtlas = createAtlas();

	world = new World(blockAtlas);
    genChunks(world);

	shader = new Shader();
	shader->createFromFile("../GlProject/shader/world.vs", "../GlProject/shader/world.fs");

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

		glUniformMatrix4fv(shader->getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(shader->getViewLocation(), 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

        textureAtlas->getTexture()->use();

        world->draw(shader->getModelLocation());

        Shader::clearShader();

		//Finish Drawing
		window->swapBuffers();

//		t.elapsedMs();
	}

	return 0;
}