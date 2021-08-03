#include "lua/Lua.h"

#include "NoiseFromLua.h"

std::vector<noise::module::Module*> NoiseFromLua::build(sol::table noise) {
	std::vector<noise::module::Module*> modules;
	parseNoise(modules, noise);
	return std::move(modules);
}

noise::module::Module* NoiseFromLua::parseNoise(std::vector<noise::module::Module*>& modules, sol::table noise) {
	std::string type = noise["module"];
	
	// Modifer Modules
	if (type == "abs") {
		auto module = new noise::module::Abs();
		
		modules.push_back(module);
		return module;
	}
	else if (type == "clamp") {
		auto module = new noise::module::Clamp();
		module->SetBounds(noise.get_or<float>("low", noise::module::DEFAULT_CLAMP_LOWER_BOUND),
			noise.get_or<float>("high", noise::module::DEFAULT_CLAMP_UPPER_BOUND));
		
		modules.push_back(module);
		return module;
	}
	else if (type == "curve") {
		auto module = new noise::module::Exponent();
		
		module->SetExponent(noise.get_or<float>("exponent", noise::module::DEFAULT_EXPONENT));
		
		modules.push_back(module);
		return module;
	}
	else if (type == "invert") {
		auto module = new noise::module::Invert();
		
		modules.push_back(module);
		return module;
	}
	else if (type == "scale_bias") {
		auto module = new noise::module::ScaleBias();
		sol::table source = noise["source"];
		
		auto mod = parseNoise(modules, source);
		module->SetSourceModule(0, *mod);
		
		module->SetScale(noise.get_or<float>("scale", noise::module::DEFAULT_SCALE));
		module->SetBias(noise.get_or<float>("bias", noise::module::DEFAULT_BIAS));
		
		modules.push_back(module);
		return module;
	}
		// Combiner Modules
	else if (type == "add") {
		auto module = new noise::module::Add();
		sol::table sources = noise["sources"];
		
		auto mod0 = parseNoise(modules, sources[1]);
		auto mod1 = parseNoise(modules, sources[2]);
		module->SetSourceModule(0, *mod0);
		module->SetSourceModule(1, *mod1);
		
		modules.push_back(module);
		return module;
	}
	else if (type == "max") {
		auto module = new noise::module::Max();
		sol::table sources = noise["sources"];
		
		auto mod0 = parseNoise(modules, sources[1]);
		auto mod1 = parseNoise(modules, sources[2]);
		module->SetSourceModule(0, *mod0);
		module->SetSourceModule(1, *mod1);
		
		modules.push_back(module);
		return module;
	}
	else if (type == "min") {
		auto module = new noise::module::Min();
		sol::table sources = noise["sources"];
		
		auto mod0 = parseNoise(modules, sources[1]);
		auto mod1 = parseNoise(modules, sources[2]);
		module->SetSourceModule(0, *mod0);
		module->SetSourceModule(1, *mod1);
		
		modules.push_back(module);
		return module;
	}
	else if (type == "multiply") {
		auto module = new noise::module::Multiply();
		sol::table sources = noise["sources"];
		
		auto mod0 = parseNoise(modules, sources[1]);
		auto mod1 = parseNoise(modules, sources[2]);
		module->SetSourceModule(0, *mod0);
		module->SetSourceModule(1, *mod1);
		
		modules.push_back(module);
		return module;
	}
	else if (type == "power") {
		auto module = new noise::module::Power();
		sol::table sources = noise["sources"];
		
		auto mod0 = parseNoise(modules, sources[1]);
		auto mod1 = parseNoise(modules, sources[2]);
		module->SetSourceModule(0, *mod0);
		module->SetSourceModule(1, *mod1);
		
		modules.push_back(module);
		return module;
	}
		// Generator modules
	else if (type == "billow") {
		auto module = new noise::module::Billow();
		
		module->SetSeed(noise.get_or<float>("seed", 0));
		module->SetOctaveCount(noise.get_or<float>("octaves", noise::module::DEFAULT_BILLOW_OCTAVE_COUNT));
		module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_BILLOW_FREQUENCY));
		module->SetLacunarity(noise.get_or<float>("lacunarity", noise::module::DEFAULT_BILLOW_LACUNARITY));
		module->SetPersistence(noise.get_or<float>("persistence", noise::module::DEFAULT_BILLOW_PERSISTENCE));
		
		modules.push_back(module);
		return module;
	}
	else if (type == "checkerboard") {
		auto module = new noise::module::Checkerboard();
		
		modules.push_back(module);
		return module;
	}
	else if (type == "const") {
		auto module = new noise::module::Const();
		
		module->SetConstValue(noise.get_or<float>("value", noise::module::DEFAULT_CONST_VALUE));
		
		modules.push_back(module);
		return module;
	}
	else if (type == "cylinders") {
		auto module = new noise::module::Cylinders();
		
		module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_CYLINDERS_FREQUENCY));
		
		modules.push_back(module);
		return module;
	}
	else if (type == "ridged_multi") {
		auto module = new noise::module::RidgedMulti();
		
		module->SetSeed(noise.get_or<float>("seed", 0));
		module->SetOctaveCount(noise.get_or<float>("octaves", noise::module::DEFAULT_RIDGED_OCTAVE_COUNT));
		module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_RIDGED_FREQUENCY));
		module->SetLacunarity(noise.get_or<float>("lacunarity", noise::module::DEFAULT_RIDGED_LACUNARITY));
		
		modules.push_back(module);
		return module;
	}
	else if (type == "spheres") {
		auto module = new noise::module::Spheres();
		
		module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_SPHERES_FREQUENCY));
		
		modules.push_back(module);
		return module;
	}
	else if (type == "perlin") {
		auto module = new noise::module::Perlin();
		
		module->SetSeed(noise.get_or<float>("seed", 0));
		module->SetOctaveCount(noise.get_or<float>("octaves", noise::module::DEFAULT_PERLIN_OCTAVE_COUNT));
		module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_PERLIN_FREQUENCY));
		module->SetLacunarity(noise.get_or<float>("lacunarity", noise::module::DEFAULT_PERLIN_LACUNARITY));
		module->SetPersistence(noise.get_or<float>("persistence", noise::module::DEFAULT_PERLIN_PERSISTENCE));
		
		modules.push_back(module);
		return module;
	}
	else if (type == "voronoi") {
		auto module = new noise::module::Voronoi();
		
		module->SetSeed(noise.get_or<float>("seed", 0));
//		module->EnableDistance(noise.get_or<u32>("distance", false));
		module->SetDisplacement(noise.get_or<float>("displacement", 0));
		module->SetFrequency(noise.get_or<float>("frequency", 0));
		
		modules.push_back(module);
		return module;
	}
		// Selector Modules
	else if (type == "blend") {
		auto module = new noise::module::Blend();
		sol::table sources = noise["sources"];
		
		auto mod0 = parseNoise(modules, sources[1]);
		auto mod1 = parseNoise(modules, sources[2]);
		auto control = parseNoise(modules, noise["control"]);
		module->SetSourceModule(0, *mod0);
		module->SetSourceModule(1, *mod1);
		module->SetSourceModule(1, *control);
		
		modules.push_back(module);
		return module;
	}
	else if (type == "select") {
		auto module = new noise::module::Select();
		sol::table sources = noise["sources"];
		
		auto mod0 = parseNoise(modules, sources[1]);
		auto mod1 = parseNoise(modules, sources[2]);
		auto control = parseNoise(modules, noise["control"]);
		module->SetSourceModule(0, *mod0);
		module->SetSourceModule(1, *mod1);
		module->SetSourceModule(1, *control);
		
		modules.push_back(module);
		return module;
	}
		// Transformer Modules
	else if (type == "turbulence") {
		auto module = new noise::module::Turbulence();
		sol::table source = noise["source"];
		
		auto mod0 = parseNoise(modules, source);
		module->SetSourceModule(0, *mod0);
		module->SetPower(noise.get_or<float>("power", noise::module::DEFAULT_TURBULENCE_POWER));
		module->SetFrequency(noise.get_or<float>("frequency", noise::module::DEFAULT_TURBULENCE_FREQUENCY));
		module->SetRoughness(noise.get_or<float>("roughness", noise::module::DEFAULT_TURBULENCE_ROUGHNESS));
		
		modules.push_back(module);
		return module;
	}
	
	else if (type == "scale_point") {
		auto module = new noise::module::ScalePoint();
		sol::table source = noise["source"];
		
		auto mod0 = parseNoise(modules, source);
		module->SetSourceModule(0, *mod0);
		module->SetXScale(noise.get_or<float>("x_scale", 1));
		module->SetYScale(noise.get_or<float>("y_scale", 1));
		module->SetZScale(noise.get_or<float>("z_scale", 1));
		
		modules.push_back(module);
		return module;
	}
	throw std::runtime_error("Invalid noise module specified.");
}