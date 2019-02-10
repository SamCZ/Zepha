//
// Created by aurailus on 05/02/19.
//
// IMPORTANT NOTE
// The ellipses in "NetHandler host can be created"s REQUIRE statment are intentional
// see https://github.com/catchorg/Catch2/issues/874
//

#include <catch.hpp>
#include "../../../src/generic/network/NetHandler.h"

TEST_CASE("NetHandler", "[networking]") {

    SECTION("NetHandler host can be created") {

        NetHandler server(12346, 1);
        REQUIRE((server.getState() == server.HOST));
    }

    SECTION("NetHandler client can be created") {

        NetHandler client("127.0.0.1", 12347, 1, 1);
        REQUIRE((client.getState() == client.FAILED_CONNECT));
    }
}