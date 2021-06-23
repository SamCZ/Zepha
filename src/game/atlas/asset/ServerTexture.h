//
// Created by aurailus on 05/08/19.
//

#pragma once

#include "util/Types.h"

struct ServerTexture {
	ServerTexture(string name, string data, u16 width, u16 height) :
		name(name), data(data), width(width), height(height) {};
	
	string name;
	string data;
	u16 width;
	u16 height;
};

