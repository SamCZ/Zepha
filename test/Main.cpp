#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"

//
// Created by aurailus on 31/01/19.
//

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

TEST_CASE("Sanity Check", "[core]") {
    REQUIRE(1 + 1 == 2);
}

#include "generic/blocks/BlockChunk.cpp"
#include "generic/network/NetHandler.cpp"

#pragma clang diagnostic pop