#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define STB_IMAGE_IMPLEMENTATION
#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image/stb_image.h>
#include <cute_files/cute_files.h>

#pragma clang diagnostic pop

#include "StartGame.h"

/*
 * Zepha, designed, developed, and created by Nicole Collings
 * This is my child, and I hope you like it.
 * Copyright 2018 - present Auri Collings, All Rights Reserved.
 */

enum class CB {
	A, B, _END
};


template<typename... Args>
void call(std::function<void(vec<any>)> cb, const Args&... args) {
	vec<any> vec = { args... };
	cb(vec);
}

int main(int argc, char* argv[]) {
	return StartGame(argc, argv);
}