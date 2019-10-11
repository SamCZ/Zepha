//
// Created by aurailus on 15/05/19.
//

#include <catch.hpp>
#include <iostream>
#include "../../src/def/item/SelectionBox.h"

TEST_CASE("Test Intersection", "math") {
    SelectionBox s({0, 0, 0}, {1, 1, 1});
    
    REQUIRE(s.intersects({0.5, 1, 0.5}, {}) == 1);
    REQUIRE(s.intersects({0.5, 0, 0.5}, {}) == 2);

    REQUIRE(s.intersects({0.5, 0.5, 0}, {}) == 3);
    REQUIRE(s.intersects({0.5, 0.5, 1}, {}) == 4);

    REQUIRE(s.intersects({0, 0.5, 0.5}, {}) == 5);
    REQUIRE(s.intersects({1, 0.5, 0.5}, {}) == 6);

    REQUIRE(s.intersects({2, 2, 2}, {}) == 0);
}