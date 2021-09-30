#pragma once

#include "util/Types.h"
#include "game/atlas/asset/AtlasTexture.h"

/**
 * Stores information about a font,
 * like the texture, UVs and widths of characters.
 */

class Font {
public:
	Font(const AtlasTexture& texture);
	
	u16 getCharWidth(char c);
	vec4 getCharUVs(char c);
	
	constexpr static u16 C_ROWSIZE = 18;
	constexpr static u16 C_COUNT = 95;
	constexpr static u16 C_WIDTH = 7;
	constexpr static u16 C_HEIGHT = 9;
	
private:
	AtlasTexture texture;
	array<u16, C_COUNT + 1> charWidths {};
};
