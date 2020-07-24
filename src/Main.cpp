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

#pragma clang diagnostic pop

#include "StartGame.h"

int main(int argc, char* argv[]) {

//    sol::state lua;
//    lua.open_libraries(sol::lib::base);
//
//    lua.script(R"(
//        number = 32
//    )");
//
//    unsigned short n = lua.get<unsigned short>("number");
//    std::cout << n << std::endl;


    return StartGame(argc, argv);
}

