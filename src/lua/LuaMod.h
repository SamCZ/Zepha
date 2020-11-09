//
// Created by aurailus on 03/08/19.
//

#pragma once

#include <vector>
#include <string>

class PacketView;

class LuaMod {
	public:
	struct File {
		std::string path;
		std::string file;
	};
	
	struct Config {
		std::string name;
		std::string description;
		std::string version;
		std::vector<std::string> depends;
	};
	
	std::vector<File> files{};
	Config config{};
	std::string modPath;
	std::string serialized;
	
	static LuaMod fromPacket(PacketView& p);
};