#include <fstream>
#include <iostream>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>

#include "Mod.h"

#include "util/net/Serializer.h"
#include "util/net/PacketView.h"

Mod::Mod(const std::filesystem::path& path, bool noConfig) {
	nlohmann::json json {};
	
	std::ifstream(path / "conf.json") >> json;
	
	rawPath = path;
	
	if (!noConfig) {
		if (!json.contains("name")) throw std::invalid_argument("Config is missing 'name'.");
		if (!json.contains("identifier")) throw std::invalid_argument("Config is missing 'identifier'.");
		if (!json.contains("description")) throw std::invalid_argument("Config is missing 'description'.");
		if (!json.contains("version")) throw std::invalid_argument("Config is missing 'version'.");
		if (!json.contains("main")) throw std::invalid_argument("Config is missing 'main'.");
		
		name = json.at("name");
		identifier = json.at("identifier");
		description = json.at("description");
		version = json.at("version");
	}
	
	if (json.contains("dependencies")) {
		nlohmann::json::object_t deps = json.at("dependencies");
		for (const let& it : deps) {
			const string& identifier = it.first;
			const Version& version = it.second;
			dependencies.emplace(identifier, version);
		}
	}
	
	std::filesystem::recursive_directory_iterator iter(path / "script");
	for (let& file : iter) {
		if (!file.is_regular_file() || file.path().extension() != ".lua") continue;
		std::ifstream fileStream(file.path());
		string fileStr = string((std::istreambuf_iterator<char>(fileStream)), std::istreambuf_iterator<char>());
		string rawPath = std::filesystem::relative(file.path(), path / "script").string();
		string relativePath = identifier + "/" + rawPath.substr(0, rawPath.size() - 4);
		
		files.emplace(relativePath, fileStr);
	}
	
	if (json.contains("main") && json.at("main").is_object()) {
		nlohmann::json::object_t main = json.at("main");
		for (const let& it : main) {
			const string& type = it.first;
			const string& path = identifier + "/" + static_cast<string>(it.second);
			
			if (type != "client" && type != "server")
				throw std::invalid_argument("Unrecognized main specifier '" + type + "' specified.");
			
			if (files.find(path) == files.end())
				throw std::invalid_argument("main file '" + path + "' not found.");
			
			if (type == "client") clientMain = path;
			else if (type == "server") serverMain = path;
		}
	}
	else {
		string main = identifier + "/" + (json.contains("main") && json.at("main").is_string()
			? static_cast<string>(json.at("main")) : "main");
		clientMain = main;
		serverMain = main;
	}
}

Mod::Mod(PacketView& p) {
	string comp = p.d.read<string>();
	string serialized = gzip::decompress(comp.data(), comp.length());
	Deserializer d(serialized);
	
	d.read(name).read(identifier).read(description).read(version).read(clientMain);

	vec<string> deps = d.read<vec<string>>();
	for (let& dep : deps) {
		usize space = dep.find(' ');
		dependencies.emplace(dep.substr(0, space - 1), dep.substr(space));
	}
	
	while (!d.atEnd()) {
		string path, contents;
		d.read(path).read(contents);
		files.emplace(path, contents);
	}
}

const string& Mod::serialize() const {
	if (!serialized.empty()) return serialized;
	
	vec<string> serializedDeps;
	for (let& pair : dependencies) serializedDeps.emplace_back(pair.first + ' ' + pair.second);
	
	Serializer s;
	s.append(name)
	 .append(identifier)
	 .append(description)
	 .append(version)
	 .append(clientMain)
	 .append(serializedDeps);

	for (let& pair : files) {
		s.append(pair.first).append(pair.second);
	}

	serialized = gzip::compress(s.data.c_str(), s.data.length());
	return serialized;
}
