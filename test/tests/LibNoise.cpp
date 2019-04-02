//
// Created by aurailus on 22/02/19.
//

#include <catch.hpp>
#include <iostream>
#include <noise/noise.h>

TEST_CASE("Noise Sanity Check", "noise") {
    using namespace noise;

    module::Perlin myModule;
    double value = myModule.GetValue(1.25, 0.75, 0.5);

    double diff = fabs(value - 0.686347);

    REQUIRE(diff < 0.1);
}