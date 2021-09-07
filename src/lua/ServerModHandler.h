#pragma once

#include <list>
#include <functional>

#include "Mod.h"

class ServerSubgame;

class ServerModHandler {
public:
	void loadMods(ServerSubgame& defs, const string& path);
	
	void executeMods(std::function<void(string)> run);
	
	vec<string> modOrder {};
	std::unordered_map<string, Mod> mods {};
private:
	void loadAssets(ServerSubgame& defs);
	
	void sortMods();
	
//	void serializeMods();
};
