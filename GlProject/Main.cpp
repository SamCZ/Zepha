#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "engine/Window.h"
#include "engine/graphics/Mesh.h"
#include "engine/Entity.h"
#include "engine/graphics/Shader.h"
#include "engine/Camera.h"
#include "engine/graphics/Texture.h"
#include "mesh/MeshData.h"
#include "mesh/MeshGenerator.h"
#include "engine/Timer.h"

Window* window;
Shader* shader;
std::vector<Entity*> entities;
Camera* camera;

Texture* brickTexture;
Texture* dirtTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void makeEntities() {

	int array[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE];
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int j = 0; j < CHUNK_SIZE; j++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				array[i][j][k] = (j < 8) ? 1 : 0;
			}
		}
	}

    auto meshGen = new MeshGenerator();
    MeshData *m = meshGen->build(array);
    delete meshGen;

	Mesh* mesh = new Mesh();
	mesh->create(&m->vertices[0], &m->indices[0], (int)m->vertices.size(), (int)m->indices.size());
	for (int i = -16; i < 16; i++) {
		for (int j = -16; j < 16; j++) {
			auto *chunk = new Entity();
			chunk->create(mesh);
			chunk->setPosition(glm::vec3(i * CHUNK_SIZE, 0, j * CHUNK_SIZE));
			chunk->setScale(0.5);
			entities.push_back(chunk);
		}
	}
}

int main() {
    //Window
    window = new Window(1366, 768);
    window->initialize();

    //Create camera
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0), -90.0f, 0.0f, 10.0f, 0.1f);

    //Load Textures
	brickTexture = new Texture((char*)"../Textures/brick.png");
	brickTexture->load();

	dirtTexture = new Texture((char*)"../Textures/default_dirt.png");
    dirtTexture->load();

	//Create entities
    makeEntities();

    //Create shader
	shader = new Shader();
	shader->createFromFile("../GlProject/shader/world.vs", "../GlProject/shader/world.fs");

	glm::mat4 projectionMatrix = glm::perspective(45.0f, window->getBufferWidth() / window->getBufferHeight(), 0.1f, 100.0f);

    glEnable(GL_CULL_FACE);

	//Game Loop
	while (!window->getShouldClose()) {
	    auto now = (GLfloat)glfwGetTime();
	    deltaTime = now - lastTime;
	    lastTime = now;

		//Get & Handle Input
		glfwPollEvents();
		window->update();

		camera->keyControl(window->getKeysArray(), deltaTime);
		camera->mouseControl(window->getDeltaX(), window->getDeltaY());

		//Clear Window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->useShader();

		glUniformMatrix4fv(shader->getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(shader->getViewLocation(), 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));

        dirtTexture->use();

		for (auto &entity : entities) {
			glUniformMatrix4fv(shader->getModelLocation(), 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));
			entity->draw();
        }

        Shader::clearShader();

		//Finish Drawing
		window->swapBuffers();
	}

	return 0;
}