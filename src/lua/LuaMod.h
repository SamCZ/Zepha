#pragma once

#include "util/Types.h"

class PacketView;

/**
 * Holds the name, description, version and dependencies
 * of a lua mod, as well as its scripts.
 */

class LuaMod {
public:
	struct File {
		File(string path, string file): path(path), file(file) {};
		
		string path;
		string file;
	};
	
	struct Config {
		string name;
		string description;
		string version;
		vec<string> depends;
	};
	
	LuaMod() = default;
	
	/** Creates a new lua mod from a packet containing mod data. */
	LuaMod(PacketView& p);
	
	/** The mod's source files. */
	vec<File> files {};
	
	/** The mod's configuration data. */
	Config config {};
	
	/** The mod's filesystem path. */
	string modPath;
	
	/** The serialized mod, populated on the server. */
	string serialized;
};