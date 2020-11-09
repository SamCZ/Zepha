//
// Created by aurailus on 24/08/19.
//

#include "Mesh.h"

void Mesh::cleanup() {
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (IBO != 0) glDeleteBuffers(1, &IBO);
	
	IBO = 0;
	VBO = 0;
	VAO = 0;
	indCount = 0;
}

void Mesh::genArrays(GLuint vboLength, GLuint iboLength, const void* verticesPtr, const void* indicesPtr) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboLength, indicesPtr, GL_STATIC_DRAW);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vboLength, verticesPtr, GL_STATIC_DRAW);
}

void Mesh::createVertexAttrib(GLuint offset, GLuint size, GLenum type, GLsizei stride, const void* pointer) {
	glEnableVertexAttribArray(offset);
	if (type == GL_INT)
		glVertexAttribIPointer(offset, size, type, stride, pointer);
	else
		glVertexAttribPointer(offset, size, type, GL_FALSE, stride, pointer);
}

void Mesh::draw() const {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	
	glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, nullptr);
}

Mesh::~Mesh() {
	cleanup();
}