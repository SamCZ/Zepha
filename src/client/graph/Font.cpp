#include "Font.h"

#include "game/atlas/asset/AtlasTexture.h"

Font::Font(const sptr<AtlasTexture>& texture) : texture(texture) {
	charWidths[0] = 2;
	const let data = texture->getBytes();
	
	for (u16 i = 1; i < C_COUNT + 1; i++) {
		u16vec2 charPos(i % C_ROWSIZE * C_WIDTH, i / C_ROWSIZE * C_HEIGHT);
		
		u16 width = 0;
		for (u16 j = 0; j < C_WIDTH; j++) {
			bool empty = true;
			
			for (u16 k = 0; k < C_HEIGHT; k++) {
				u32 xx = charPos.x + j;
				u32 yy = charPos.y + k;
				
				u32 offset = yy * texture->getSize().x * 4 + xx * 4 + 3;
				
				if (data[offset] != 0) {
					empty = false;
					break;
				}
			}
			
			if (!empty) {
				width = j;
			}
		}
		
		charWidths[i] = width;
	}
}

u16 Font::getCharWidth(char c) {
	u16 index = static_cast<u16>(c) - 32;
	if (index >= C_COUNT) return charWidths[C_COUNT];
	return charWidths[index];
}

vec4 Font::getCharUVs(char c) {
	u16 index = static_cast<u16>(c) - 32;
	if (index >= C_COUNT) index = C_COUNT;
	
	vec4 texUVs = texture->getUVPos();
	u16vec2 texSize = texture->getSize();
	u16vec2 charPos((index % C_ROWSIZE) * C_WIDTH, index / C_ROWSIZE * C_HEIGHT);
	
	return {
		texUVs.x + charPos.x * (texUVs.z - texUVs.x) / texSize.x,
		texUVs.y + charPos.y * (texUVs.w - texUVs.y) / texSize.y,
		texUVs.x + (charPos.x + getCharWidth(c) + 1) * (texUVs.z - texUVs.x) / texSize.x,
		texUVs.y + (charPos.y + C_HEIGHT) * (texUVs.w - texUVs.y) / texSize.y
	};
}
