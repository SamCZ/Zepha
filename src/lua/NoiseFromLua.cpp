#include "lua/Lua.h"

#include "NoiseFromLua.h"

FastNoise::SmartNode<> NoiseFromLua::parse(sol::table table) {
	using namespace FastNoise;
	
	let type = table.get<string>("module");
//	std::cout << type << std::endl;
	
	/**
	 * Produces a constant value, regardless of the position.
	 *
	 * @param value - The value to produce, default 0.
	 */
	 
	if (type == "const") {
		let module = New<Constant>();
		module->SetValue(table.get_or<f32>("value", 0));
		return module;
	}
	
	/**
	 * Applies the input position to the output using the factors and offsets provided.
	 * All values default to zero.
	 *
	 * @param x_factor - The factor that the x value should influence the output.
	 * @param y_factor - The factor that the y value should influence the output.
	 * @param z_factor - The factor that the z value should influence the output.
	 * @param x_offset - The offset applied to the x value.
	 * @param y_offset - The offset applied to the y value.
	 * @param z_offset - The offset applied to the z value.
	 */
	
	if (type == "position_output") {
		let module = New<PositionOutput>();
		
		module->Set<Dim::X>(table.get_or("x_factor", 0), table.get_or("x_offset", 0));
		module->Set<Dim::Y>(table.get_or("y_factor", 0), table.get_or("y_offset", 0));
		module->Set<Dim::Z>(table.get_or("z_factor", 0), table.get_or("z_offset", 0));
		
		return module;
	}
	
	/**
	 * Adds two sources, or a source and a scalar.
	 *
	 * @param sources - Two sources to add. Supply this or `source` and `scalar`.
	 * @param source - A single source, if supplied, also specify `scalar`.
	 * @param scalar - A scalar value to add to `source`.
	 */
	
	if (type == "add") {
		let module = New<Add>();
		
		if (table.get<sol::optional<sol::table>>("sources")) {
			module->SetLHS(parse(table["sources"][1]));
			module->SetRHS(parse(table["sources"][2]));
		}
		else {
			module->SetLHS(parse(table["source"]));
			module->SetRHS(table.get_or<f32>("scalar", 1));
		}
		
		return module;
	}
	
	/**
	 * Subtracts two sources, or a source and a scalar.
	 *
	 * @param sources - Two sources to subtract. Supply this or `source` and `scalar`.
	 * @param source - A single source, if supplied, also specify `scalar`.
	 * @param scalar - A scalar value to add to `source`.
	 */
	
	if (type == "subtract") {
		let module = New<Subtract>();
		
		if (table.get<sol::optional<sol::table>>("sources")) {
			module->SetLHS(parse(table["sources"][1]));
			module->SetRHS(parse(table["sources"][2]));
		}
		else {
			module->SetLHS(parse(table["source"]));
			module->SetRHS(table.get_or<f32>("scalar", 1));
		}
		
		return module;
	}
	
	/**
	 * Multiplies two sources, or a source and a scalar.
	 *
	 * @param sources - Two sources to multiply. Supply this or `source` and `scalar`.
	 * @param source - A single source, if supplied, also specify `scalar`.
	 * @param scalar - A scalar value to multiply `source` by.
	 */
	
	if (type == "multiply") {
		let module = New<Multiply>();
		
		if (table.get<sol::optional<sol::table>>("sources")) {
			module->SetLHS(parse(table["sources"][1]));
			module->SetRHS(parse(table["sources"][2]));
		}
		else {
			module->SetLHS(parse(table["source"]));
			module->SetRHS(table.get_or<f32>("scalar", 1));
		}
		
		return module;
	}
	
	/**
	 * Divides two sources, or a source and a scalar.
	 *
	 * @param sources - Two sources to divide. Supply this or `source` and `scalar`.
	 * @param source - A single source, if supplied, also specify `scalar`.
	 * @param scalar - A scalar value to divide `source` by.
	 */
	
	if (type == "divide") {
		let module = New<Divide>();
		
		if (table.get<sol::optional<sol::table>>("sources")) {
			module->SetLHS(parse(table["sources"][1]));
			module->SetRHS(parse(table["sources"][2]));
		}
		else {
			module->SetLHS(parse(table["source"]));
			module->SetRHS(table.get_or<f32>("scalar", 1));
		}
		
		return module;
	}
	
	/**
	 * Blends between two sources, weighted by the factor provided.
	 *
	 * @param sources - The two sources to blend.
	 * @param factor - The factor to blend by, default 0.5.
	 */
	
	if (type == "fade") {
		let module = New<Fade>();
		
		module->SetA(parse(table["sources"][1]));
		module->SetB(parse(table["sources"][2]));
		module->SetFade(table.get_or<f32>("factor", 0.5));
		
		return module;
	}
	
	/**
	 * Selects the minimum between two sources, or a source and a scalar.
	 * The two can also be smoothed between.
	 *
	 * @param sources - Two sources to choose the minimum of. Supply this or `source` and `scalar`.
	 * @param source - A single source, if supplied, also specify `scalar`.
	 * @param scalar - A scalar value as the minimum.
	 * @param smoothness - The smoothness of the transition, defaults to 0.
	 */
	
	if (type == "min") {
		let smoothness = table.get_or("smoothness", 0);
		
		if (smoothness > 0) {
			let module = New<MinSmooth>();
			let sources = table["sources"];
			
			module->SetSmoothness(smoothness);
			if (sources.valid()) {
				module->SetLHS(parse(sources[1]));
				module->SetRHS(parse(sources[2]));
			}
			else {
				module->SetLHS(parse(table["source"]));
				module->SetRHS(table.get_or<f32>("scalar", 1));
			}
			
			return module;
		}
		else {
			let module = New<Min>();
			let sources = table["sources"];
			
			if (sources.valid()) {
				module->SetLHS(parse(sources[1]));
				module->SetRHS(parse(sources[2]));
			}
			else {
				module->SetLHS(parse(table["source"]));
				module->SetRHS(table.get_or<f32>("scalar", 1));
			}
			
			return module;
		}
	}
	
	/**
	 * Selects the maximum between two sources, or a source and a scalar.
	 * The two can also be smoothed between.
	 *
	 * @param sources - Two sources to choose the maximum of. Supply this or `source` and `scalar`.
	 * @param source - A single source, if supplied, also specify `scalar`.
	 * @param scalar - A scalar value as the maximum.
	 * @param smoothness - The smoothness of the transition, defaults to 0.
	 */
	
	if (type == "max") {
		let smoothness = table.get_or("smoothness", 0);
		
		if (smoothness > 0) {
			let module = New<MaxSmooth>();
			let sources = table["sources"];
			
			module->SetSmoothness(smoothness);
			if (sources.valid()) {
				module->SetLHS(parse(sources[1]));
				module->SetRHS(parse(sources[2]));
			}
			else {
				module->SetLHS(parse(table["source"]));
				module->SetRHS(table.get_or<f32>("scalar", 1));
			}
			
			return module;
		}
		else {
			let module = New<Max>();
			let sources = table["sources"];
			
			if (sources.valid()) {
				module->SetLHS(parse(sources[1]));
				module->SetRHS(parse(sources[2]));
			}
			else {
				module->SetLHS(parse(table["source"]));
				module->SetRHS(table.get_or<f32>("scalar", 1));
			}
			
			return module;
		}
	}
	
	/**
	 * Clamps the source between two scalar values. Can also be smoothed.
	 *
	 * @param source - The source to clamp.
	 * @param min - The minimum value, defaults to 0.
	 * @param max - The maximum value, defaults to 1.
	 * @param smoothness - The smoothness of the transition, defaults to 0.
	 */
	
	if (type == "max") {
		let source = parse(table["source"]);
		let smoothness = table.get_or<f32>("smoothness", 0);
		
		if (smoothness > 0) {
			let minModule = New<MinSmooth>();
			minModule->SetLHS(source);
			minModule->SetRHS(table.get_or<f32>("min", 0));
			minModule->SetSmoothness(smoothness);
			
			let maxModule = New<MaxSmooth>();
			maxModule->SetLHS(minModule);
			maxModule->SetRHS(table.get_or<f32>("max", 1));
			maxModule->SetSmoothness(smoothness);
			
			return maxModule;
		}
		else {
			let minModule = New<Min>();
			minModule->SetLHS(source);
			minModule->SetRHS(table.get_or<f32>("min", 0));
			
			let maxModule = New<Max>();
			maxModule->SetLHS(minModule);
			maxModule->SetRHS(table.get_or<f32>("max", 1));
			
			return maxModule;
		}
	}
	
	/**
	 * Generates Simplex Noise.
	 *
	 * @param frequency - The frequency of the noise, default 1.
	 * @param octaves - The amount of octaves the noise will have, default 3.
	 * @param lacunarity - The lacunarity of the octaves, default 0.5.
	 * @param seed - The seed **offset** to use, relative to the world seed.
	 */
	
	if (type == "simplex") {
		SmartNode<> module = New<Simplex>();
		
		let seed = table.get_or<u32>("seed", 0);
		let frequency = table.get_or<f32>("frequency", 1);
		let octaves = table.get_or<f32>("octaves", 3);
		let persistence = table.get_or<f32>("persistence", 0.5);
		let lacunarity = table.get_or<f32>("lacunarity", 0.5);
		
		if (frequency != 1) {
			let scaleModule = New<DomainScale>();
			scaleModule->SetSource(module);
			scaleModule->SetScale(frequency);
			module = scaleModule;
		}
		
		if (octaves > 1) {
			let fractalModule = New<FractalFBm>();
			fractalModule->SetSource(module);
			fractalModule->SetOctaveCount(octaves);
			fractalModule->SetLacunarity(lacunarity);
			fractalModule->SetGain(persistence);
			module = fractalModule;
		}
		
		if (seed != 0) {
			let seedOffsetModule = New<SeedOffset>();
			seedOffsetModule->SetSource(module);
			seedOffsetModule->SetOffset(seed);
			module = seedOffsetModule;
		}
		
		return module;
	}
	
	/**
	 * Scales the domain by axis.
	 *
	 * @param source - The source to scale.
	 * @param x_scale - The scaling to apply to the x axis.
	 * @param y_scale - The scaling to apply to the y axis.
	 * @param z_scale - The scaling to apply to the z axis.
	 */
	
	if (type == "scale") {
		let module = New<DomainAxisScale>();
		
		module->SetSource(parse(table["source"]));
		module->SetScale<Dim::X>(table.get_or<f32>("x_scale", 1));
		module->SetScale<Dim::Y>(table.get_or<f32>("y_scale", 1));
		module->SetScale<Dim::Z>(table.get_or<f32>("z_scale", 1));
		
		return module;
	}
	
	throw std::runtime_error("Invalid module name: " + type);
}