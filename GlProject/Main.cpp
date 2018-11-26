#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Mesh.h"
#include "Entity.h"

//Window dimensions
const GLint width = 800;
const GLint height = 600;

std::vector<Entity*> entities;
Entity* triangle;

//Conversion therapy
const float toRadians = 3.14159265f / 180.0f;

GLuint shader;
GLint uModel, uProjection;

bool  triDirection = true; //Right
float triOffset = 0;
float triMaxOffset = 0.7;
float triMoveStep = 0.01;
float triAngle = 0;
float triAngleStep = 1.0f;
bool  triScaleDir = true;
float triScaleMin = 0.1f;
float triScaleMax = 0.8f;
float triScaleCur = 0.4f;
float triScaleStep = 0.01f;

//Vertex Shader
static const char* vShader = "\n\
#version 330																\n\
																			\n\
layout (location = 0) in vec3 pos;											\n\
																			\n\
uniform mat4 model;															\n\
uniform mat4 projection;													\n\
																			\n\
out vec4 color;																\n\
																			\n\
void main() {																\n\
    gl_Position = projection * model * vec4(pos, 1.0);						\n\
	color = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);	 				            \n\
}																			\n\
";

//Fragment Shader
static const char* fShader = "\n\
#version 330							                                    \n\
                                                                            \n\
in vec4 color;																\n\
																			\n\
out vec4 fragColor;  					                                    \n\
										                                    \n\
void main() {							                                    \n\
	fragColor = color;						                                \n\
}										                                    \n\
";

void addShader(GLuint program, const char* shaderCode, GLenum shaderType) {
	GLuint shader = glCreateShader(shaderType);
	const GLchar* code;
	code = shaderCode;

	GLint codeLength;
	codeLength = static_cast<GLint>(strlen(shaderCode));

	glShaderSource(shader, 1, &code, &codeLength);
	glCompileShader(shader);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

	if (!result) {
		const char* shaderTypeString = (shaderType == GL_VERTEX_SHADER) ? "vertex" : "fragment";

		glGetShaderInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("Error compiling the %s shader:\n'%s'\n", shaderTypeString, eLog);
		return;
	}

	glAttachShader(program, shader);
}

void compileShaders() {
	shader = glCreateProgram();
	
	if (!shader) {
		printf("Error creating the shader program.");
		return;
	}

	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("Error linking program: '%s'\n", eLog);
		return;
	}

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);

	if (!result) {
		glGetProgramInfoLog(shader, sizeof(eLog), nullptr, eLog);
		printf("Error validating program: '%s'\n", eLog);
		return;
	}

	uModel = glGetUniformLocation(shader, "model");
	uProjection = glGetUniformLocation(shader, "projection");
}

void createTriangle() {
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

	auto tri = new Entity();
	tri->create(mesh);
	tri->setPosition(glm::vec3(0, 0, -2.5f));
	tri->setScale(0.4);

	triangle = tri;

	entities.push_back(triangle);

	auto tri2 = new Entity();
	tri2->create(mesh);
	tri2->setPosition(glm::vec3(1, -0.5, -2.5f));
	tri2->setScale(0.4);

	entities.push_back(tri2);

	auto tri3 = new Entity();
	tri3->create(mesh);
	tri3->setPosition(glm::vec3(-0.5, 0.5, -2.5f));
	tri3->setScale(0.4);

	entities.push_back(tri3);
}

int main() {
	//Initialize GLFW
	if (!glfwInit()) {
		printf("GLFW init failed");
		glfwTerminate();
		return 1;
	}

	//Set up GLFW window properties

	//Version 3.2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Compatibility - No Backwards compat, only forwards
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//Create the window
	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL Learning", nullptr, nullptr);

	if (!window) {
		printf("GLFW window failed");
		glfwTerminate();
		return 1;
	}

	//Get buffer size information (inner window array size)
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(window, &bufferWidth, &bufferHeight);

	//Set context for GLEW to our window
	glfwMakeContextCurrent(window);

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	//Initialize GLEW
	GLenum error;
	if ((error = glewInit()) != GLEW_OK) {
		printf("GLEW init failed.");
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		glfwDestroyWindow(window);
		glfwTerminate();
		return 1;
	}

	//Setup viewport (draw) size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glEnable(GL_DEPTH_TEST);

	//Create triangle
	createTriangle();
	compileShaders();

	glm::mat4 projectionMatrix = glm::perspective(45.0f, (GLfloat)bufferWidth / (GLfloat)bufferHeight, 0.1f, 100.0f);

	//Game Loop
	while (!glfwWindowShouldClose(window)) {
		//Get & Handle Input
		glfwPollEvents();

		if (triDirection /*Right*/) {
			triOffset += triMoveStep;
		}
		else /*Left*/ {
			triOffset -= triMoveStep;
		}

		if (std::abs(triOffset) >= triMaxOffset) triDirection = !triDirection;

		triAngle += triAngleStep;
		if (triAngle >= 360) {
            triAngle -= 360;
		}

		if (triScaleDir /*Increasing*/) {
			triScaleCur += triScaleStep;
		}
		else /*Decreasing*/ {
			triScaleCur -= triScaleStep;
		}

		if (triScaleCur > triScaleMax || triScaleCur < triScaleMin) triScaleDir = !triScaleDir;

//		triangle->setScale(triScaleCur);
		triangle->setPosition(glm::vec3(triOffset, 0, -2.5f));
		triangle->setScale(glm::vec3(0.4, 0.4, 1));

		//Clear Window
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

		for (auto &entity : entities) {
			glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));
			entity->draw();
        }

		glUseProgram(0);

		//Finish Drawing - Swap buffers
		glfwSwapBuffers(window);
	}

	return 0;
}