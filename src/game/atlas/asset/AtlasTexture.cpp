#include "AtlasTexture.h"

#include "game/atlas/TextureAtlas.h"

vec4 AtlasTexture::getUVPos() const {
	const uvec2 size = holds_alternative<ByteData>(shr->data) ?
		get<ByteData>(shr->data).size : get<CropData>(shr->data).size;
	const uvec2 pos = holds_alternative<ByteData>(shr->data) ?
		shr->pos : shr->pos + get<CropData>(shr->data).pos;
	const f32 divX = atlas.getCanvasSize().x, divY = atlas.getCanvasSize().y;
	
	return vec4(pos.x / divX, pos.y / divY, (pos.x + size.x) / divX, (pos.y + size.y) / divY);
}

vec<u8> AtlasTexture::getBytes() const {
	if (std::holds_alternative<ByteData>(shr->data)) return get<ByteData>(shr->data).data;
	throw std::runtime_error("cant");
}

optional<u32> AtlasTexture::getTintInd() const {
	if (!shr->tintData) return std::nullopt;
	return shr->tintData->first;
}

optional<AtlasTexture> AtlasTexture::getTintMask() const {
	if (!shr->tintData) return std::nullopt;
	return *shr->tintData->second.get();
}

void AtlasTexture::setPersistent(bool persistent) {
	shr->persistent = persistent;
}

void AtlasTexture::setTintData(u32 tintInd, const AtlasTexture& tintMask) {
	shr->tintData = {{ tintInd, make_unique<AtlasTexture>(tintMask) }};
}

bool AtlasTexture::shouldBeRemoved() {
	return shr.use_count() == 1 && !shr->persistent;
}

AtlasTexture::~AtlasTexture() {
	if (shr.use_count() == 2 && !shr->persistent) atlas.alertUnused(shr->identifier);
}