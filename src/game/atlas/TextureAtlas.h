#pragma once

#include <unordered_map>

#include "util/Types.h"
#include "client/graph/Texture.h"

class AtlasRef;

class TextureAtlas {
public:
	struct RawTexData { u8* data; uvec2 size; };
	
	TextureAtlas() = default;
	
	explicit TextureAtlas(uvec2 size);
	
	void update();
	
	vec<sptr<AtlasRef>> loadDirectory(const string& path, bool base = true, bool recurse = true);
	
	sptr<AtlasRef> loadImage(const string& path, const string& name, bool base = false);
	
	sptr<AtlasRef> addImage(const string& name, bool base, u16vec2 size, u8* data);
	
	sptr<AtlasRef> operator[](const string& name);
	
	vec4 getPixel(const sptr<AtlasRef>& ref, ivec2 pos);
	
	sptr<AtlasRef> generateCrackImage(const string& name, u8 crackLevel);
	
	ivec2 canvasSize;
	ivec2 canvasTileSize;
	
	Texture texture {};
	vec<u8> atlasData;
	
	u32 textureSlotsUsed = 0;
	u32 maxTextureSlots = 0;

private:
	sptr<AtlasRef> generateTexture(string req);
	
	RawTexData getBytesOfTex(const string& name);
	
	RawTexData getBytesAtPos(uvec2 pos, uvec2 size);
	
	optional<u16vec2> findImageSpace(u16vec2 tileSize);
	
	void createMissingTexture();
	
	void updateAtlas(u16vec2 pos, u16vec2 size, u8* data);
	
	void deleteImage(sptr<AtlasRef> ref);
	
	vec<bool> empty;
	std::unordered_map<string, sptr<AtlasRef>> textures;
};

