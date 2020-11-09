//
// Created by aurailus on 2020-04-05.
//

#include "MapGenProps.h"

MapGenProps::MapGenProps(unsigned int seed) : seed(seed) {
	temperatureBase.SetSeed(seed);
	temperatureBase.SetFrequency(0.02);
	temperatureBase.SetOctaveCount(4);
	temperatureTurbulence.SetSeed(seed);
	temperatureTurbulence.SetSourceModule(0, temperatureBase);
	temperatureTurbulence.SetRoughness(4);
	temperatureTurbulence.SetFrequency(0.2);
	temperature.SetSourceModule(0, temperatureTurbulence);
	temperature.SetScale(0.35);
	temperature.SetBias(0.25);
	
	humidityBase.SetSeed(seed + 1);
	humidityBase.SetFrequency(0.02);
	humidityBase.SetOctaveCount(4);
	humidityTurbulence.SetSeed(seed + 1);
	humidityTurbulence.SetSourceModule(0, humidityBase);
	humidityTurbulence.SetRoughness(4);
	humidityTurbulence.SetFrequency(0.2);
	humidity.SetSourceModule(0, humidityTurbulence);
	humidity.SetScale(0.5);
	humidity.SetBias(0.5);
	
	roughnessBase.SetSeed(seed - 1);
	roughnessBase.SetFrequency(0.02);
	roughnessBase.SetOctaveCount(4);
	roughnessTurbulence.SetSeed(seed - 1);
	roughnessTurbulence.SetSourceModule(0, roughnessBase);
	roughnessTurbulence.SetRoughness(4);
	roughnessTurbulence.SetFrequency(0.2);
	roughness.SetSourceModule(0, roughnessTurbulence);
	roughness.SetScale(0.5);
	roughness.SetBias(0.5);
}
