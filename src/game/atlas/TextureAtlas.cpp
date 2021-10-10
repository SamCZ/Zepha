#include <algorithm>
#include <stb_image.h>

#include "TextureAtlas.h"

#include "util/Log.h"
#include "util/Util.h"
#include "game/atlas/asset/AtlasTexture.h"

TextureAtlas::TextureAtlas(uvec2 size) :
	canvasSize(size),
	canvasTileSize(size / 16u),
	tilesTotal(canvasTileSize.x * canvasTileSize.y),
	tiles(canvasTileSize.x * canvasTileSize.y, false) {
	
	// Get GPU texture capabilites and log it.
	i32 maxTexSize, texUnits;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    std::cout << Log::info << "This GPU's max texture size is: " << maxTexSize << "px^2." << Log::endl;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texUnits);
    std::cout << Log::info << "This GPU supports " << texUnits << " texture units." << Log::endl;
	
	// Initialize the texture atlas.
	texture.loadFromBytes(&vec<u8>(size.x * size.y * 4)[0], size.x, size.y);
	createMissingTexture();
}

vec<AtlasRef> TextureAtlas::addDirectory(const std::filesystem::path& path, bool persistent) {
	vec<AtlasRef> refs {};
	for (let file : std::filesystem::recursive_directory_iterator(path))
		if (file.path().extension() == ".png") refs.push_back(addFile(file.path(), persistent));
	return refs;
}

AtlasRef TextureAtlas::addFile(const std::filesystem::path& path, bool persistent) {
	i32 width, height;
	u8* rawData = stbi_load(path.string().data(), &width, &height, nullptr, 4);
	vec<u8> data(rawData, rawData + width * height * 4);
	free(rawData);
	let ref = addBytes(path.stem().string(), persistent, u16vec2(width, height), data);
	return ref;
}

AtlasRef TextureAtlas::addBytes(const string& identifier, bool persistent, u16vec2 size, vec<u8> data) {
	let tileSize = u16vec2(glm::ceil(vec2(size) / 16.f));
	let posOpt = findAtlasSpace(tileSize);
	if (!posOpt) throw std::runtime_error("Failed to find space in the dynamic definition atlas.");
	u16vec2 pos = *posOpt * static_cast<u16>(16);
	
	tilesUsed += tileSize.x * tileSize.y;
	textures.erase(identifier);
	AtlasTexture* raw = new AtlasTexture(*this, identifier, pos, size, data);
	AtlasRef ref = sptr<AtlasTexture>(raw, [this](AtlasTexture* tex) { removeTexture(tex); });
	if (persistent) persistentTextures.insert(ref);
	addTexture(ref);
	
	return ref;
}

AtlasRef TextureAtlas::operator[](const string& identifier) {
	const let tex = get(identifier);
	if (tex->getIdentifier() != "_missing") return tex;
	
	let data = texBuilder.build(identifier);
	throw std::runtime_error("unimplemented");
//	let texture = addBytes(identifier, false, data.size, data.getBytes());
//	if (data.tintMask) texture->setTintData(*data.tintInd,
//		addBytes(identifier + ":tint", false, data.size, *data.tintMask));
//	return texture;
}

AtlasRef TextureAtlas::get(const string& identifier) const {
	if (textures.count(identifier)) return textures.at(identifier).lock();
	return textures.at("_missing").lock();
}

const uvec2 TextureAtlas::getCanvasSize() {
	return canvasSize;
}

optional<u16vec2> TextureAtlas::findAtlasSpace(u16vec2 tileSize) {
	for (u16 j = 0; j < canvasTileSize.y - (tileSize.y - 1); j++) {
		for (u16 i = 0; i < canvasTileSize.x - (tileSize.x - 1); i++) {
			bool space = true;
			
			for (u16 k = 0; k < tileSize.y; k++) {
				for (u16 l = 0; l < tileSize.x; l++) {
					if (tiles[(j + k) * canvasTileSize.x + (i + l)]) {
						space = false;
						break;
					}
				}
				
				if (!space) break;
			}
			
			if (space) {
				for (u16 k = 0; k < tileSize.y; k++)
					for (u16 l = 0; l < tileSize.x; l++)
						tiles[(j + k) * canvasTileSize.x + (i + l)] = true;
				return u16vec2(i, j);
			}
		}
	}
	
	return {};
}

void TextureAtlas::createMissingTexture() {
	let data = vec<u8>(16 * 4 * 16);
	for (u16 i = 0; i < 16 * 16; i++) {
		u8 m = 0;
		if ((i % 16 < 8) ^ ((i / 16) < 8)) m = 255;
		
		data[i * 4 + 0] = m;
		data[i * 4 + 1] = 0;
		data[i * 4 + 2] = m;
		data[i * 4 + 3] = 255;
	}
	
	addBytes("_missing", true, u16vec2(16), data);
}

void TextureAtlas::addTexture(const AtlasRef& tex) {
	let pos = tex->getPos();
	let size = tex->getSize();
	textures.insert({ tex->getIdentifier(), std::weak_ptr(tex) });
	texture.updateTexture(pos.x, pos.y, size.x, size.y, tex->getBytes().data());
}

void TextureAtlas::removeTexture(const AtlasTexture* tex) {
	std::cout << tex->getIdentifier() << std::endl;
	textures.erase(tex->getIdentifier());
	
	let tilePos = tex->getTilePos();
	let tileSize = tex->getTileSize();
	
	tilesUsed -= tileSize.x * tileSize.y;
	
	for (u16 x = tilePos.x; x < tilePos.x + tileSize.x; x++)
		for (u16 y = tilePos.y; y < tilePos.y + tileSize.y; y++)
			tiles[y * canvasTileSize.x + x] = false;
		
	// For debugging
	vec<u8> clear(tileSize.x * 16 * tileSize.y * 16 * 4);
	texture.updateTexture(tilePos.x * 16, tilePos.y * 16, tileSize.x * 16, tileSize.y * 16, clear.data());
	
	delete tex;
}

const u32 TextureAtlas::getTilesUsed() {
	return tilesUsed;
}

const u32 TextureAtlas::getTilesTotal() {
	return tilesTotal;
}

const Texture& TextureAtlas::getTexture() {
	return texture;
}