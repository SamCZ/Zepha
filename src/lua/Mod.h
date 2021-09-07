#pragma once

#include <filesystem>
#include <unordered_map>

#include "util/Types.h"

class PacketView;

/**
 * Holds the properties and source files for a Lua mod.
 */

class Mod {
public:
	/** Stores a version, this will be a struct in the future. */
	typedef string Version;
	
	/** Default constructor. */
	Mod() = default;
	
	/** Creates a new lua mod from the mod path on the filesystem. */
	Mod(const std::filesystem::path& path, bool noConfig = false);
	
	/** Creates a new lua mod from a packet containing mod data. */
	Mod(PacketView& p);
	
	const string& serialize() const;
	
	/** The mod's name. */
	string name;
	
	/** The mod's identifier. */
	string identifier;
	
	/** The mod's description. */
	string description;
	
	/** The version of the mod. */
	Version version;
	
	/** The mod's dependencies and versions. */
	std::unordered_map<string, Version> dependencies {};
	
	/** Main entrypoint for the server. On the client this will be blank. */
	string serverMain {};
	
	/** Main entrypoint for the client, may be a blank string indicating no client side code. */
	string clientMain {};
	
	/** The mod's source files. */
	std::unordered_map<string, string> files {};
	
	/** The mod's path on the filesystem. */
	std::filesystem::path rawPath {};
	
private:
	
	/** The serialized mod, only present on the server. */
	mutable string serialized {};
};