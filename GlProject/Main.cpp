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
#include "Camera.h"

Window* window;
Shader* shader;
std::vector<Entity*> entities;
Camera* camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

void makeEntities() {
//	unsigned int indices[] {
//		0, 3, 1,
//		1, 3, 2,
//		2, 3, 0,
//		0, 1, 2
//	};
//
//	GLfloat vertices[] = {
//		-1.0f, -1.0f, -0.5f,
//		 0.0f, -1.0f,  1.0f,
//		 1.0f, -1.0f, -0.5f,
//		 0.0f,  1.0f,  0.0f
//	};

    unsigned int indices[] {
        //Bottom
        0, 2, 3,
        3, 1, 0,
        //Top
        4, 6, 7,
        7, 5, 4,
        //Left
        0, 1, 5,
        5, 4, 0,
        //Right
        7, 3, 2,
        2, 6, 7,
        //Back
        0, 2, 6,
        6, 4, 0,
        //Front
        1, 3, 7,
        7, 5, 1
    };

    GLfloat vertices[] = {
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f, -0.5f,  0.5f,

        -0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f,  0.5f,
         0.5f,  0.5f, -0.5f,
         0.5f,  0.5f,  0.5f,
    };

    Mesh* mesh = new Mesh();
    mesh->create(vertices, indices, (sizeof(vertices)/sizeof(*vertices)), (sizeof(indices)/sizeof(*indices)));

    for (auto y = -1; y < 1; y++) {
        for (auto i = -1; i < 2; i++) {
            for (auto j = -4; j < -2; j++) {

                auto *tri = new Entity();
                tri->create(mesh);
                tri->setPosition(glm::vec3(i * 1.45, (y + 0.5) * 1.6, j));
                tri->setScale(0.7);

                entities.push_back(tri);
            }
        }
    }
}

int main() {
    //Window
    window = new Window(1366, 768);
    window->initialize();

    //Create camera
    camera = new Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0, 1, 0), -90.0f, 0.0f, 10.0f, 0.1f);

	//Create entities
    makeEntities();

    //Create shader
	shader = new Shader();
	shader->createFromFile("../GlProject/Shaders/world.vs", "../GlProject/Shaders/world.fs");


	glm::mat4 projectionMatrix = glm::perspective(45.0f, window->getBufferWidth() / window->getBufferHeight(), 0.1f, 100.0f);

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

		for (auto &entity : entities) {
			glUniformMatrix4fv(shader->getModelLocation(), 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));
			entity->setAngle(*entity->getAngle() + 0.5f);
			entity->draw();
        }

        Shader::clearShader();

		//Finish Drawing
		window->swapBuffers();
	}

	return 0;
}