//
// Created by aurailus on 13/08/19.
//

#include <cmath>

#include "Font.h"

#include "game/atlas/asset/AtlasRef.h"
#include "game/atlas/TextureAtlas.h"

Font::Font(TextureAtlas& atlas, std::shared_ptr<AtlasRef> tex) :
	fontTex(std::move(tex)),
	atlasSize(atlas.canvasSize) {
	
	getCharWidths(atlas);
}

u16 Font::getCharWidth(char c) {
	unsigned int index = static_cast<unsigned int>(c) - 32;
	if (index >= C_COUNT) return charWidths[C_COUNT];
	return charWidths[index];
}

void Font::getCharWidths(TextureAtlas& atlas) {
	auto& data = atlas.atlasData;
	
	charWidths[0] = 2;
	
	for (u16 i = 1; i < C_COUNT + 1; i++) {
		glm::vec2 charPos = { i % 18 * C_WIDTH, std::floor(i / 18) * C_HEIGHT };
		
		u32 xBase = static_cast<u32>(fontTex->rawPos.x) + static_cast<u32>(charPos.x);
		u32 yBase = static_cast<u32>(fontTex->rawPos.y) + static_cast<u32>(charPos.y);
		
		u16 width = 0;
		
		for (u16 j = 0; j < C_WIDTH; j++) {
			bool empty = true;
			for (u16 k = 0; k < C_HEIGHT; k++) {
				u32 xx = xBase + j;
				u32 yy = yBase + k;
				
				u32 offset = yy * static_cast<u32>(atlasSize.x) * 4 + xx * 4 + 3;
				
				if (data[offset] != 0) {
					empty = false;
					break;
				}
			}
			if (!empty) width = static_cast<u16>(j);
		}
		
		charWidths[i] = width;
	}
}

vec4 Font::getCharUVs(char c) {
	u16 index = static_cast<u16>(c) - 32;
	if (index >= C_COUNT) index = C_COUNT;
	
	vec2 charPos = { (index % 18) * C_WIDTH, std::floor(index / 18) * C_HEIGHT };
	vec4 uv = {
		fontTex->uv.x + (charPos.x) / atlasSize.x,
		fontTex->uv.y + (charPos.y) / atlasSize.y,
		fontTex->uv.x + (charPos.x + getCharWidth(c) + 1) / atlasSize.x,
		fontTex->uv.y + (charPos.y + C_HEIGHT) / atlasSize.y
	};
	
	return uv;
}
