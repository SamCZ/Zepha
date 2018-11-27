#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Entity.h"
#include "Shader.h"

Window mainWindow;
std::vector<Entity*> entities;
Shader* shader;

bool  triDirection = true; //Right
float triOffset = 0;
float triMaxOffset = 0.7;
float triMoveStep = 0.01;

void makeEntities() {
	unsigned int indices[] {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, -0.5f,
		 0.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -0.5f,
		 0.0f,  1.0f,  0.0f
	};

    Mesh* mesh = new Mesh();
    mesh->create(vertices, indices, (sizeof(vertices)/sizeof(*vertices)), (sizeof(indices)/sizeof(*indices)));

    for (auto y = -1; y < 1; y++) {
        for (auto i = -1; i < 2; i++) {
            for (auto j = -4; j < -2; j++) {

                auto *tri = new Entity();
                tri->create(mesh);
                tri->setPosition(glm::vec3(i, y + 0.25, j));
                tri->setScale(0.3);

                entities.push_back(tri);
            }
        }
    }
}

int main() {
    //Window
    mainWindow = Window(800, 600);
    mainWindow.initialize();

	//Create entities
    makeEntities();

    //Create shader
	shader = new Shader();
	shader->createFromFile("../GlProject/Shaders/world.vs", "../GlProject/Shaders/world.fs");

	glm::mat4 projectionMatrix = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//Game Loop
	while (!mainWindow.getShouldClose()) {
		//Get & Handle Input
		glfwPollEvents();

		if (triDirection /*Right*/) {
			triOffset += triMoveStep;
		}
		else /*Left*/ {
			triOffset -= triMoveStep;
		}

		if (std::abs(triOffset) >= triMaxOffset) triDirection = !triDirection;

//		triangle->setPosition(glm::vec3(triOffset, 0, -2.5f));
//		triangle->setScale(glm::vec3(0.4, 0.4, 1));

		//Clear Window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->useShader();

		glUniformMatrix4fv(shader->getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		for (auto &entity : entities) {
			glUniformMatrix4fv(shader->getModelLocation(), 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));

			entity->getPosition()->x += triMoveStep * (triDirection ? 1 : -1);
			entity->draw();
        }

        Shader::clearShader();

		//Finish Drawing - Swap buffers
		mainWindow.swapBuffers();
	}

	return 0;
}