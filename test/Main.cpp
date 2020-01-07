#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION
#define CATCH_CONFIG_MAIN
#pragma clang diagnostic pop

#include <catch2/catch.hpp>

TEST_CASE("Catch2 Library", "[core]") {
    REQUIRE(true);
}

#include "tests/TestSpace.cpp"
#include "tests/TestBlockChunk.cpp"
#include "tests/TestSerializer.cpp"