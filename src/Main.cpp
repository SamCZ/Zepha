/*
 * Zepha, designed, developed, and created by Nicole Collings
 * This is my child, and I hope you like it.
 * Copyright 2018 - present Nicole Collings, All Rights Reserved.
 */

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define STB_IMAGE_IMPLEMENTATION
#define CUTE_FILES_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image/stb_image.h>
#include <cute_files/cute_files.h>
#include <world/dim/chunk/Chunk.h>

#pragma clang diagnostic pop

//#include "StartGame.h"

/**
 * Main entrance point to the program. (Am I really describing what the main function is?)
 * Intentionally kept minimal to allow for testing. Real startup logic is done in StartGame.
 *
 * @param argc - Argument array length
 * @param argv - Argument array
 * @returns - A numerical value indicating exit status.
 */
 
#include <GLFW/glfw3.h>
#include <iostream>

int main(int argc, char* argv[]) {
//	return StartGame(argc, argv);

	Chunk a = Chunk();
	for (int i = 0; i < 128; i++) {
		const auto rnd = floor(rand() * 100);
		for (int j = 0; j < 32; j++) {
			a.setBlock(i * 32 + j, rnd);
			a.setBiome(i * 32 + j, rnd * 2);
		}
	}
	
	glfwInit();
	
	auto start = glfwGetTime() * 1000;
	
	for (int i = 0; i < 1000; i++) {
		auto b = Chunk(a);
//		auto l = a.compress();
//		std::cout << b.isGenerated() << std::endl;
	}
	
	auto end = glfwGetTime() * 1000;
	
	std::cout << (end - start) << std::endl;
	
//	Chunk b = Chunk(a.compress());
//	b.decompress();
//
////	std::cout << a.compress().length() << std::endl;
//
//	for (int i = 0; i < 4096; i++) assert(a.getBlock(i) == b.getBlock(i));
}
