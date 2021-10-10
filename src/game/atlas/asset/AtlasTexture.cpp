#include "AtlasTexture.h"

#include "game/atlas/TextureAtlas.h"

vec4 AtlasTexture::getUVPos() const {
	const uvec2 size = holds_alternative<ByteData>(data) ? get<ByteData>(data).size : get<CropData>(data).size;
	const uvec2 pos = holds_alternative<ByteData>(data) ? this->pos : this->pos + get<CropData>(data).pos;
	const f32 divX = atlas.getCanvasSize().x;
	const f32 divY = atlas.getCanvasSize().y;
	
	return vec4(pos.x / divX, pos.y / divY, (pos.x + size.x) / divX, (pos.y + size.y) / divY);
}

vec<u8> AtlasTexture::getBytes() const {
	if (std::holds_alternative<ByteData>(data)) return get<ByteData>(data).data;
	throw std::runtime_error("cant");
}

optional<u32> AtlasTexture::getTintInd() const {
	if (!tintData) return std::nullopt;
	return tintData->first;
}

optional<AtlasRef> AtlasTexture::getTintMask() const {
	if (!tintData) return std::nullopt;
	return tintData->second;
}

void AtlasTexture::setTintData(u32 tintInd, const AtlasRef& tintMask) {
	tintData = {{ tintInd, tintMask }};
}