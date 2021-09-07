#include <fstream>
#include <filesystem>
#include <stb_image.h>
#include <unordered_map>
#include <gzip/compress.hpp>

#include "ServerModHandler.h"

#include "game/ServerSubgame.h"

void ServerModHandler::loadMods(ServerSubgame& defs, const string& path) {
	let mod = Mod { std::filesystem::path(path) / "../../../assets/base" };
	mods.emplace(mod.identifier, mod);
	
	std::filesystem::recursive_directory_iterator iter(path);
	for (let& dir : iter) {
		if (!dir.is_directory() || !std::filesystem::exists(dir.path() / "conf.json")) continue;
		try {
			let mod = Mod { dir.path() };
			mods.emplace(mod.identifier, mod);
		}
		catch (std::invalid_argument e) {
			std::cout << Log::err << "Failed to load mod '"
				<< dir.path().string() << "\n" << e.what() << Log::endl;
			continue;
		}
	}
	
	loadAssets(defs);
	sortMods();
}

void ServerModHandler::executeMods(std::function<void(string)> run) {
	for (let& mod : modOrder) run(mods[mod].serverMain);
}

void ServerModHandler::loadAssets(ServerSubgame& defs) {
	for (const let& mod : mods) {
		let path = std::filesystem::path(mod.second.rawPath) / "textures";
		if (std::filesystem::exists(path)) {
			std::filesystem::recursive_directory_iterator iter(path);
			
			for (let& file : iter) {
				if (file.path().extension() != ".png") continue;
				
				int width, height;
				u8* data = stbi_load(file.path().string().data(), &width, &height, nullptr, 4);
				string str(reinterpret_cast<char*>(data), static_cast<usize>(width * height * 4));
				std::string comp = gzip::compress(str.data(), str.length());
				free(data);
				
				let name = file.path().filename().string();
				name = name.substr(0, name.size() - 4);
				
				defs.assets.textures.emplace_back(
					mod.second.identifier + ":" + name,
					comp, static_cast<u16>(width), static_cast<u16>(height));
			}
		}
		
		path = std::filesystem::path(mod.second.rawPath) / "models";
		if (std::filesystem::exists(path)) {
			std::filesystem::recursive_directory_iterator iter(path);
			
			for (let& file : iter) {
				if (file.path().extension() != ".b3d") continue;
				
				std::ifstream t(file.path());
				std::stringstream buffer;
				buffer << t.rdbuf();
				
				defs.assets.models.emplace_back(SerializedModel {
					mod.second.identifier + ":" + file.path().filename().string(),
					buffer.str(), "b3d" });
			}
		}
	}
}

void ServerModHandler::sortMods() {
	modOrder.emplace_back("zepha:base");
	
	std::function<void(Mod&, const string&, std::unordered_map<string, string>&)> insertMod =
		[&](Mod& mod, const string& parent, std::unordered_map<string, string>& pending) {
		for (let& o : modOrder) if (o == mod.identifier) return;
		let circIt = pending.find(mod.identifier);
		if (circIt != pending.end()) {
			throw std::runtime_error("Circular dependency, '" + circIt->second + "' requires '" + mod.identifier +
				"', but " + mod.identifier + " is required elsewhere.");
		}
		pending.emplace(mod.identifier, parent);
		
		for (let& dep : mod.dependencies) {
			if (dep.first == "zepha") continue;
			
			let depIt = mods.find(dep.first);
			if (depIt == mods.end()) throw std::runtime_error(
				"Mod '" + mod.identifier + "' is missing dependency '" + dep.first + "'.");
			insertMod(depIt->second, mod.identifier, pending);
		}
		
		modOrder.emplace_back(mod.identifier);
		pending.erase(mod.identifier);
	};
	
	for (let& mod : mods) {
		std::unordered_map<string, string> pending {};
		insertMod(mod.second, "TOP LEVEL", pending);
	}
}