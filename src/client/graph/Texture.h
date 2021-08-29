//
// Created by aurailus on 29/11/18.
//

#pragma once

#include <string>
#include "util/GL.h"

class Texture {
	public:
	Texture() = default;
	
	explicit Texture(const std::string& file);
	
	void loadFromFile(std::string file);
	
	void loadFromBytes(unsigned char* bytes, int width, int height, GLint interpolation = GL_NEAREST,
		GLint repeatMode = GL_CLAMP_TO_EDGE);
	
	void updateTexture(int x, int y, int width, int height, unsigned char* bytes);
	
	void use(GLuint position = 0);
	
	void clear();
	
	~Texture();
	
	protected:
	unsigned int textureID = 0;
	int width = 0;
	int height = 0;
	int bitDepth = 0;
	
	std::string fileLocation;
};

