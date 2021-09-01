//
// Created by aurailus on 16/06/19.
//

#pragma once

#include <glm/mat4x4.hpp>

#include "util/GL.h"

struct GuiUniforms {
	glm::mat4 matrix;
	
	GLint ortho;
	GLint model;
	
	GLint bones;
	GLint clipBounds;
};
