#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define STB_IMAGE_IMPLEMENTATION
#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <cute_files.h>

#pragma clang diagnostic pop

#include "StartGame.h"
#include <iostream>
#include "client/CallbackGroup.h"

/*
 * Zepha, designed, developed, and created by Nicole Collings
 * This is my child, and I hope you like it.
 * Copyright 2018 - present Auri Collings, All Rights Reserved.
 */

enum class CB {
	A, B, _END
};

int main(int argc, char* argv[]) {
	return StartGame(argc, argv);
/*
	CallbackGroup<CB, u32> cbs;
	
	u32 cbA1 = cbs.bind(CB::A, [&](u32 i) { std::cout << "a1 " << i << std::endl; });
	u32 cbA2 = cbs.bind(CB::A, [&](u32 i) { std::cout << "a2 " << i << std::endl; });
	u32 cbA3 = cbs.bind(CB::A, [&](u32 i) { std::cout << "a3 " << i << std::endl; });
	
	cbs.bind(CB::B, [&](u32 i) { std::cout << "b1 " << i << std::endl; });
	cbs.bind(CB::B, [&](u32 i) { std::cout << "b2 " << i << std::endl; });

	cbs.call(CB::A, "hi!");
	cbs.call(CB::A, 1);
	cbs.call(CB::A, 2);
	
	cbs.call(CB::B, 2);
	cbs.call(CB::B, 1);
	
	cbs.unbind(CB::A, cbA1);
	cbs.unbind(CB::A, cbA2);
	
	cbs.call(CB::A, 3);
	
	cbs.unbind(CB::A, cbA3);
	
	cbs.call(CB::A, 4);*/
}