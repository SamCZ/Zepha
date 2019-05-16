#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define STB_IMAGE_IMPLEMENTATION
#define CATCH_CONFIG_MAIN
#pragma clang diagnostic pop

#include <catch.hpp>

TEST_CASE("Sanity Check", "[core]") {
    REQUIRE(1 + 1 == 2);
}

#include "tests/BlockChunk.cpp"
#include "tests/NetHandler.cpp"
#include "tests/Serializer.cpp"
#include "tests/Packet.cpp"
#include "tests/LibNoise.cpp"
#include "tests/Dimension.cpp"
#include "tests/List.cpp"
#include "tests/Intersects.cpp"