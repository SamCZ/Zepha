#include <gzip/decompress.hpp>

#include "LuaMod.h"

#include "util/net/PacketView.h"

LuaMod::LuaMod(PacketView& p) {
	auto serialized = p.d.read<string>();
	string mod = gzip::decompress(serialized.c_str(), serialized.length());
	
	vec<string> depends;
	string name, description, version;
	
	Deserializer d(mod);
	d.read(name).read(description).read(version).read(depends);
	config = { name, description, version, depends };
	
	while (!d.atEnd()) {
		string path, contents;
		d.read(path).read(contents);
		files.emplace_back(path, contents);
	}
}