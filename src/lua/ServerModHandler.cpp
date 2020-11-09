//
// Created by aurailus on 2020-02-19.
//

#include <sstream>
#include <fstream>
#include <json/json.hpp>
#include <gzip/compress.hpp>
#include <stb_image/stb_image.h>
#include <cute_files/cute_files.h>

#include "ServerModHandler.h"

#include "game/ServerSubgame.h"
#include "util/net/Serializer.h"

void ServerModHandler::loadMods(ServerSubgame& defs, const std::string& path) {
	auto modDirs = findModDirectories(path);
	mods = initializeLuaMods(modDirs);
	
	loadTextures(defs, mods);
	loadModels(defs, mods);
	
	organizeDependencies(mods);
	serializeMods(mods);
}

void ServerModHandler::executeMods(std::function<void(std::string)> run) {
	for (LuaMod& mod : mods) {
		if (mod.config.name == "base") {
			run(mod.config.name + "/init");
			break;
		}
	}
	
	for (LuaMod& mod : mods) {
		if (mod.config.name != "base") run(mod.config.name + "/init");
	}
}

const std::vector<LuaMod>& ServerModHandler::cGetMods() const {
	return mods;
}

std::list<std::string> ServerModHandler::findModDirectories(const std::string& path) {
	std::list<std::string> modDirs{ path + "/../../../assets/base" };
	std::list<std::string> dirsToScan{ path };
	
	cf_dir_t dir;
	
	while (!dirsToScan.empty()) {
		std::string dirStr = *dirsToScan.begin();
		dirsToScan.erase(dirsToScan.begin());
		bool isModFolder = false;
		
		cf_dir_open(&dir, dirStr.c_str());
		
		std::list<std::string> subDirs;
		
		while (dir.has_next) {
			cf_file_t scannedFile;
			cf_read_file(&dir, &scannedFile);
			
			if (strncmp(scannedFile.name, ".", 1) == 0) {
				cf_dir_next(&dir);
				continue;
			}
			
			if (scannedFile.is_dir) subDirs.emplace_back(scannedFile.path);
			else if (std::string_view(scannedFile.name).size() == 9 &&
			         std::string_view(scannedFile.name).rfind("conf.json") == 0) {
				
				isModFolder = true;
				break;
			}
			
			cf_dir_next(&dir);
		}
		
		cf_dir_close(&dir);
		
		if (isModFolder) modDirs.push_back(dirStr);
		else for (const std::string& s : subDirs) dirsToScan.push_back(s);
	}
	
	return std::move(modDirs);
}

std::vector<LuaMod> ServerModHandler::initializeLuaMods(const std::list<std::string>& modDirs) {
	using nlohmann::json;
	
	std::vector<LuaMod> mods{};
	cf_dir_t dir;
	
	for (const std::string& modDir : modDirs) {
		std::string root = modDir + "/script";
		
		std::list<std::string> dirsToScan{ root };
		std::list<std::string> luaFiles{};
		
		while (!dirsToScan.empty()) {
			std::string dirStr = *dirsToScan.begin();
			dirsToScan.erase(dirsToScan.begin());
			
			cf_dir_open(&dir, dirStr.c_str());
			
			while (dir.has_next) {
				cf_file_t scannedFile;
				cf_read_file(&dir, &scannedFile);
				
				if (strncmp(scannedFile.name, ".", 1) != 0) {
					if (scannedFile.is_dir) dirsToScan.emplace_back(scannedFile.path);
					else {
						char* dot = strrchr(scannedFile.path, '.');
						if (dot && (strncmp(dot, ".lua", 4) == 0)) luaFiles.emplace_back(scannedFile.path);
					}
				}
				cf_dir_next(&dir);
			}
			cf_dir_close(&dir);
		}
		
		LuaMod mod{};
		mod.modPath = modDir;
		auto& conf = mod.config;
		
		std::ifstream i(modDir + "/conf.json");
		json j{};
		i >> j;
		
		auto depends = j["depends"];
		if (strncmp(depends.type_name(), "array", 5) == 0) {
			for (auto& it : depends) {
				if (strncmp(it.type_name(), "string", 6) == 0) {
					conf.depends.push_back(static_cast<std::string>(it));
				}
			}
		}
		
		conf.name = j["name"];
		conf.description = j["description"];
		conf.version = j["version"];
		
		for (std::string& file : luaFiles) {
			size_t rootPos = file.find(root);
			std::string modPath = file;
			assert(rootPos != std::string::npos);
			
			std::ifstream t(file);
			std::string fileStr = std::string((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
			
			modPath.erase(rootPos, root.length());
			modPath.insert(0, conf.name);
			modPath.resize(modPath.size() - 4);
			
			LuaMod::File f{ modPath, fileStr };
			mod.files.push_back(f);
		}
		mods.push_back(mod);
	}
	return mods;
}

void ServerModHandler::loadTextures(ServerSubgame& defs, const std::vector<LuaMod>& mods) {
	cf_dir_t dir;
	for (const LuaMod& mod : mods) {
		std::string root = mod.modPath + "/textures";
		
		std::list<std::string> dirsToScan{ root };
		
		while (!dirsToScan.empty()) {
			std::string dirStr = *dirsToScan.begin();
			dirsToScan.erase(dirsToScan.begin());
			
			if (!cf_file_exists(dirStr.c_str())) continue;
			cf_dir_open(&dir, dirStr.c_str());
			
			while (dir.has_next) {
				cf_file_t scannedFile;
				cf_read_file(&dir, &scannedFile);
				
				if (strncmp(scannedFile.name, ".", 1) != 0) {
					if (scannedFile.is_dir) dirsToScan.emplace_back(scannedFile.path);
					else {
						char* dot = strrchr(scannedFile.path, '.');
						if (dot && strncmp(dot, ".png", 4) == 0) {
							
							std::string name = std::string(scannedFile.name).substr(0,
								std::string(scannedFile.name).size() - 4);
							name.insert(0, mod.config.name + ":");
							
							int width, height;
							unsigned char* data = stbi_load(scannedFile.path, &width, &height, nullptr, 4);
							std::string str(reinterpret_cast<char*>(data),
								static_cast<unsigned long>(width * height * 4));
							std::string comp = gzip::compress(str.data(), str.length());
							free(data);
							
							defs.assets.textures.push_back({ std::move(name), comp, static_cast<unsigned int>(width),
								static_cast<unsigned int>(height) });
						}
					}
				}
				
				cf_dir_next(&dir);
			}
			cf_dir_close(&dir);
		}
	}
}

void ServerModHandler::loadModels(ServerSubgame& defs, const std::vector<LuaMod>& mods) {
	cf_dir_t dir;
	for (const LuaMod& mod : mods) {
		std::string root = mod.modPath + "/models";
		
		std::list<std::string> dirsToScan{ root };
		
		while (!dirsToScan.empty()) {
			std::string dirStr = *dirsToScan.begin();
			dirsToScan.erase(dirsToScan.begin());
			
			if (!cf_file_exists(dirStr.c_str())) continue;
			cf_dir_open(&dir, dirStr.c_str());
			
			while (dir.has_next) {
				cf_file_t scannedFile;
				cf_read_file(&dir, &scannedFile);
				
				if (strncmp(scannedFile.name, ".", 1) != 0) {
					if (scannedFile.is_dir) dirsToScan.emplace_back(scannedFile.path);
					else {
						char* dot = strrchr(scannedFile.path, '.');
						if (dot && strncmp(dot, ".b3d", 4) == 0) {
							
							std::string name = std::string(scannedFile.name).substr(0,
								std::string(scannedFile.name).size() - 4);
							name.insert(0, mod.config.name + ":");
							
							std::ifstream t(scannedFile.path);
							std::stringstream buffer;
							buffer << t.rdbuf();
							
							defs.assets.models.push_back({ std::move(name), buffer.str(), "b3d" });
						}
					}
				}
				
				cf_dir_next(&dir);
			}
			cf_dir_close(&dir);
		}
	}
}

void ServerModHandler::organizeDependencies(std::vector<LuaMod>& mods) {
	for (int i = 0; i < mods.size(); i++) {
		LuaMod& mod = mods[i];
		auto& deps = mod.config.depends;
		
		bool modifiedList = false;
		
		for (std::string& dep : deps) {
			for (int j = 0; j < mods.size(); j++) {
				LuaMod& otherMod = mods[j];
				if (otherMod.config.name == dep) {
					if (j > i) {
						LuaMod copy = otherMod;
						mods.erase(mods.begin() + j);
						mods.insert(mods.begin() + i, copy);
						i++;
						modifiedList = true;
						break;
					}
				}
			}
		}
		
		if (modifiedList) i = -1;
	}
}

void ServerModHandler::serializeMods(std::vector<LuaMod>& mods) {
	for (LuaMod& mod : mods) {
		Serializer s = {};
		s.append(mod.config.name)
			.append(mod.config.description)
			.append(mod.config.version);
		
		std::string depends;
		bool delimiter = false;
		for (const std::string& dep : mod.config.depends) {
			if (delimiter) depends.append(",");
			else delimiter = true;
			depends.append(dep);
		}
		
		s.append(depends);
		
		for (LuaMod::File& file : mod.files) {
			s.append(file.path).append(file.file);
		}
		
		std::string comp = gzip::compress(s.data.c_str(), s.data.length());
		mod.serialized = comp;
	}
}
