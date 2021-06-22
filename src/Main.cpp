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

#include "StartGame.h"

/**
 * Main entrance point to the program. (Am I really describing what the main function is?)
 * Intentionally kept minimal to allow for testing. Real startup logic is done in StartGame.
 */

int main(int argc, char* argv[]) {
	return StartGame(argc, argv);
}
