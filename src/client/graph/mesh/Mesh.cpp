#include "Mesh.h"

void Mesh::draw() const {
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	
	glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, nullptr);
}

Mesh::~Mesh() {
	if (VAO != 0) glDeleteVertexArrays(1, &VAO);
	if (VBO != 0) glDeleteBuffers(1, &VBO);
	if (IBO != 0) glDeleteBuffers(1, &IBO);
}

void Mesh::genArrays(usize vboLength, usize iboLength, const void* vertices, const void* indices) {
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iboLength, indices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vboLength, vertices, GL_STATIC_DRAW);
}

void Mesh::createVertexAttrib(u32 offset, u32 size, GLenum type, u32 stride, const void* pointer) {
	glEnableVertexAttribArray(offset);
	if (type == GL_INT) glVertexAttribIPointer(offset, size, type, stride, pointer);
	else glVertexAttribPointer(offset, size, type, GL_FALSE, stride, pointer);
}