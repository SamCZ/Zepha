#pragma once

#include <iostream>
#include <filesystem>
#include <unordered_set>
#include <unordered_map>

#include "util/Types.h"
#include "util/StringParser.h"
#include "client/graph/Texture.h"
#include "game/atlas/TextureBuilder.h"
#include "game/atlas/asset/AtlasTexture.h"

class AtlasTexture;

/**
 * Manages loading and generating images into a single atlas texture,
 * with reference counting and runtime manipulations.
 */

class TextureAtlas {
public:
	TextureAtlas() = default;
	
	explicit TextureAtlas(uvec2 size);
	
	void update();
	
	vec<AtlasTexture> addDirectory(const std::filesystem::path& path, bool base);
	
	AtlasTexture addFile(const std::filesystem::path& path, bool base);
	
	AtlasTexture addBytes(const string& identifier, bool base, u16vec2 size, vec<u8> data);
	
	AtlasTexture operator[](const string& identifier);
	
	AtlasTexture get(const string& identifier) const;
	
	const uvec2 getCanvasSize();
	
	const u32 getTilesUsed();
	
	const u32 getTilesTotal();
	
	const Texture& getTexture();
	
	void alertUnused(const string& identifier);
private:
	void createMissingTexture();
	
	optional<u16vec2> findAtlasSpace(u16vec2 tileSize);
	
	void addTexture(const AtlasTexture& tex);
	
	void removeTexture(const AtlasTexture& tex);
	
	uvec2 canvasSize;
	uvec2 canvasTileSize;
	
	u32 tilesUsed = 0;
	u32 tilesTotal = 0;
	
	vec<bool> tiles;
	std::unordered_set<string> texturesUnused {};
	std::unordered_map<string, AtlasTexture> textures {};
	
	Texture texture {};
	const TextureBuilder texBuilder { *this };
};

