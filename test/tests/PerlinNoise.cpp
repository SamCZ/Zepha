//
// Created by aurailus on 22/02/19.
//

#include <catch.hpp>
#include <iostream>
#include "../../src/generic/helpers/PerlinNoise.h"

TEST_CASE("Perlin Range Check") {

    PerlinNoise p;

    double min = 0.5, max = 0.5;

    for (int i = 0; i < 100000; i++) {
        auto v = p.noise(i / 12.0f, i / 0.23f, i / 18.0f);
        if (v > max) max = v;
        if (v < min) min = v;
    }

    printf("Perlin Noise Exhibited Values:\nMin: %f, Max: %f\n", min, max);

    REQUIRE(min < 0.1); //Require Min is around 0
    REQUIRE(min >= 0);  //Require Min is not below 0
    REQUIRE(max > 0.9); //Require Max is around 1
    REQUIRE(min <= 1);  //Require Max is not above 1
}