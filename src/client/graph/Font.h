#pragma once

#include "util/Types.h"

class AtlasRef;
class TextureAtlas;

class Font {
public:
	Font() = default;
	
	Font(TextureAtlas& atlas, std::shared_ptr<AtlasRef> tex);
	
	u16 getCharWidth(char c);
	
	vec4 getCharUVs(char c);
	
	constexpr static u16 C_COUNT = 95;
	constexpr static u16 C_WIDTH = 7;
	constexpr static u16 C_HEIGHT = 9;
	
private:
	void getCharWidths(TextureAtlas& atlas);
	
	vec2 atlasSize {};
	
	sptr<AtlasRef> fontTex = nullptr;
	array<u16, C_COUNT + 1> charWidths {};
};
