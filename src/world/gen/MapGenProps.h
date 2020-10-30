//
// Created by aurailus on 2020-04-05.
//

#pragma once

#include <libnoise/noise.h>

class MapGenProps {
public:
    MapGenProps(unsigned int seed);

    unsigned int seed;

    noise::module::Perlin temperatureBase;
    noise::module::Turbulence temperatureTurbulence;
    noise::module::ScaleBias temperature;

    noise::module::Perlin humidityBase;
    noise::module::Turbulence humidityTurbulence;
    noise::module::ScaleBias humidity;

    noise::module::Perlin roughnessBase;
    noise::module::Turbulence roughnessTurbulence;
    noise::module::ScaleBias roughness;
};
