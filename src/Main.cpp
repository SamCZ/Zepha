#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

#pragma clang diagnostic pop

#include "StartGame.h"

/*
 * Zepha, designed, developed, and created by Auri Collings
 * This is my child, and I hope you like it.
 * Copyright 2018 - present Auri Collings, All Rights Reserved.
 */

int main(int argc, char* argv[]) {
	return StartGame(argc, argv);
}