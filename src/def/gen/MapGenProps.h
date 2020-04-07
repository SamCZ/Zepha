//
// Created by aurailus on 2020-04-05.
//

#pragma once

#include <noise/noise.h>

using namespace noise;

class MapGenProps {
public:
    MapGenProps(unsigned int seed);

    unsigned int seed;

    module::Perlin temperatureBase;
    module::Turbulence temperatureTurbulence;
    module::ScaleBias temperature;

    module::Perlin humidityBase;
    module::Turbulence humidityTurbulence;
    module::ScaleBias humidity;

    module::Perlin roughnessBase;
    module::Turbulence roughnessTurbulence;
    module::ScaleBias roughness;
};
