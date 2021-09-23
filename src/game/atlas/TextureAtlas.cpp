#include <algorithm>
#include <stb_image.h>
#include <cute_files.h>

#include "TextureAtlas.h"

#include "util/Log.h"
#include "util/Util.h"
#include "game/atlas/asset/AtlasRef.h"

TextureAtlas::TextureAtlas(uvec2 size) :
	canvasSize(size),
	canvasTileSize(size / 16u),
	atlasData(size.x * 4 * size.y),
	maxTextureSlots(canvasTileSize.x * canvasTileSize.y),
	empty(canvasTileSize.x * canvasTileSize.y, true) {

	/**
	 * Crop Function.
	 * Crops the texture to the specified dimensions and position.
	 *
	 * @param x - The left edge of the crop on the base texture.
	 * @param y - The top edge of the crop on the base texture.
	 * @param w - The width of the crop.
	 * @param h - The height of the crop.
	 * @param tex - The texture to crop.
	 */
	 
	parser.addFn<u16, u16, u16, u16, TexParserData>("crop", [](u16 x, u16 y, u16 w, u16 h, TexParserData tex) {
		let newData = vec<u8>(w * h * 4);
		for (u32 i = 0; i < w * h * 4; i++) newData[i] =
			tex.data->data[((x + i / 4 % w) + (y + i / 4 / w) * tex.data->size.x) * 4 + i % 4];

		tex.data->data = newData;
		tex.data->size = { w, h };
		return tex;
	});

	/**
	 * Multiply Function.
	 * Multiplies the texture by the specified color.
	 *
	 * @param hex - The hex code of the color to multiply the image by.
	 * @param tex - The texture to multiply.
	 */
	
	parser.addFn<string, TexParserData>("multiply", [](string hex, TexParserData tex) {
		vec4 multiple = Util::hexToColorVec(hex);
		
		for (u32 i = 0; i < tex.data->size.x * tex.data->size.y * 4; i++)
			tex.data->data[i] *= multiple[i % 4];
		
		return tex;
	});
	
	/**
	 * Literal Function.
	 * Converts raw strings to texture data by indexing the atlas.
	 *
	 * @param tex - The string of the texture to convert.
	 */

	parser.addLiteralFnCtx([](TexParser::Ctx& ctx, string tex) {
		return TexParser::Data { std::make_shared<RawTexData>(ctx.atlas.getBytesOfTex(tex)) };
	});
	
	// Get GPU texture capabilites and log it.
 
	i32 maxTexSize, texUnits;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
    std::cout << Log::info << "This GPU's max texture size is: " << maxTexSize << "px^2." << Log::endl;

    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &texUnits);
    std::cout << Log::info << "This GPU supports " << texUnits << " texture units." << Log::endl;
	
	// Initialize the texture atlas.
	
	texture.loadFromBytes(&atlasData[0], size.x, size.y);
	
	createMissingTexture();
}

vec<sptr<AtlasRef>> TextureAtlas::loadDirectory(const string& path, bool base, bool recurse) {
	vec<sptr<AtlasRef>> refs {};
	
	cf_dir_t dir;
	cf_dir_open(&dir, (path).c_str());
	
	while (dir.has_next) {
		cf_file_t file;
		cf_read_file(&dir, &file);
		std::string_view name = file.name;
		
		if (!file.is_dir && strcmp(file.ext, ".png") == 0)
			refs.push_back(loadImage(file.path, string(name.substr(0, name.size() - 4)), base));
		
		if (recurse && file.is_dir && name.data()[0] != '.') {
			let vec = loadDirectory(file.path, base, true);
			refs.insert(refs.end(), vec.begin(), vec.end());
		}
		cf_dir_next(&dir);
	}
	cf_dir_close(&dir);
	
	return refs;
}

sptr<AtlasRef> TextureAtlas::loadImage(const string& path, const string& name, bool base) {
	i32 width, height;
	u8* rawData = stbi_load(path.data(), &width, &height, nullptr, 4);
	vec<u8> data(rawData, rawData + width * height * 4);
	free(rawData);
	let ref = addImage(name, base, u16vec2(width, height), data);
	return ref;
}

void TextureAtlas::update() {
	for (let it = textures.begin(); it != textures.end();) {
		if (!it->second->base && it->second.unique()) {
			deleteImage(it->second);
			it = textures.erase(it);
		}
		else it++;
	}
}

vec4 TextureAtlas::getPixel(const sptr<AtlasRef>& ref, ivec2 pos) {
	uvec2 absPos = { ref->rawPos.x + pos.x, ref->rawPos.y + pos.y };
	u32 index = (static_cast<u32>(absPos.y) * canvasSize.x + static_cast<u32>(absPos.x)) * 4;
	
	return { atlasData[index] / 255.f, atlasData[index + 1] / 255.f,
		atlasData[index + 2] / 255.f, atlasData[index + 3] / 255.f };
}

sptr<AtlasRef> TextureAtlas::addImage(const string& name, bool base, u16vec2 size, vec<u8> data) {
	u16vec2 tileSize = u16vec2(glm::ceil(vec2(size) / 16.f));
	
	let posOpt = findImageSpace(tileSize);
	if (!posOpt) throw std::runtime_error("Failed to find space in the dynamic definition atlas.");
	u16vec2 pos = *posOpt;
	
	textureSlotsUsed += tileSize.x * tileSize.y;
	
	textures.erase(name);
	let ref = make_shared<AtlasRef>(AtlasRef {
		pos,
		tileSize,
		uvec2(pos) * 16u,
		size,
		vec4(pos.x * 16 / static_cast<f32>(canvasSize.x), pos.y * 16 / static_cast<f32>(canvasSize.y),
			(pos.x * 16 + size.x) / static_cast<f32>(canvasSize.x), (pos.y * 16 + size.y) / static_cast<f32>(canvasSize.y)),
		name,
		base
	});
	
	textures.insert({ name, ref });
	
	updateAtlas(ref->rawPos, ref->rawSize, data);
	return ref;
}

sptr<AtlasRef> TextureAtlas::generateCrackImage(const string& name, u8 crackLevel) {
	RawTexData base = getBytesOfTex(name);
	
	std::string crackStr("zeus:default:crack_" + std::to_string(crackLevel));
	RawTexData crack = getBytesOfTex(crackStr);
	
	for (int i = 0; i < base.size.x * base.size.y; i++) {
		float alpha = crack.data[i * 4 + 3] / 255.f;
		
		base.data[i * 4 + 0] = static_cast<u8>(base.data[i * 4 + 0] * (1 - alpha) + crack.data[i * 4 + 0] * alpha);
		base.data[i * 4 + 1] = static_cast<u8>(base.data[i * 4 + 1] * (1 - alpha) + crack.data[i * 4 + 1] * alpha);
		base.data[i * 4 + 2] = static_cast<u8>(base.data[i * 4 + 2] * (1 - alpha) + crack.data[i * 4 + 2] * alpha);
	}
	
	auto ref = addImage(name + "_crack_" + std::to_string(crackLevel), false, base.size, base.data);
	return ref;
}

sptr<AtlasRef> TextureAtlas::operator[](const string& name) {
	if (textures.count(name)) return textures[name];
	
	sptr<AtlasRef> gen = generateTexture(name);
	if (gen) return gen;
	
	throw std::runtime_error("Invalid texture: '" + name + "'");
}

sptr<AtlasRef> TextureAtlas::generateTexture(string req) {
	TexParserCtx ctx { *this };
	const let data = parser.parse(req, ctx);
	return addImage(req, false, data.data->size, data.data->data);
}

TextureAtlas::RawTexData TextureAtlas::getBytesOfTex(const string& name) {
	let it = textures.find(name);
	if (it != textures.end()) return getBytesAtPos(it->second->rawPos, it->second->rawSize);
	std::cout << Log::err << "Invalid base texture '" << name << "'." << Log::endl;
	let& missing = textures["_missing"];
	return getBytesAtPos(missing->rawPos, missing->rawSize);
}

TextureAtlas::RawTexData TextureAtlas::getBytesAtPos(uvec2 pos, uvec2 size) {
	RawTexData tex { vec<u8>(size.x * size.y * 4), size };
	
	for (u32 i = 0; i < size.x * size.y; i++) {
		u32 x = pos.x + (i % size.x);
		u32 y = pos.y + (i / size.x);
		
		tex.data[i * 4 + 0] = atlasData[x * 4     + y * (canvasSize.x * 4)];
		tex.data[i * 4 + 1] = atlasData[x * 4 + 1 + y * (canvasSize.x * 4)];
		tex.data[i * 4 + 2] = atlasData[x * 4 + 2 + y * (canvasSize.x * 4)];
		tex.data[i * 4 + 3] = atlasData[x * 4 + 3 + y * (canvasSize.x * 4)];
	}
	
	return tex;
}

optional<u16vec2> TextureAtlas::findImageSpace(u16vec2 tileSize) {
	for (u16 j = 0; j < canvasTileSize.y - (tileSize.y - 1); j++) {
		for (u16 i = 0; i < canvasTileSize.x - (tileSize.x - 1); i++) {
			if (empty[j * canvasTileSize.x + i]) {
				bool space = true;
				
				for (int k = 0; k < tileSize.y; k++) {
					for (int l = 0; l < tileSize.x; l++) {
						if (!empty[(j + k) * canvasTileSize.x + (i + l)]) {
							space = false;
							break;
						}
					}
					
					if (!space) break;
				}
				
				if (space) {
					for (int k = 0; k < tileSize.y; k++) {
						for (int l = 0; l < tileSize.x; l++) {
							empty[(j + k) * canvasTileSize.x + (i + l)] = false;
						}
					}
					return u16vec2(i, j);
				}
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
	
	addImage("_missing", true, u16vec2(16), data);
}

void TextureAtlas::updateAtlas(u16vec2 pos, u16vec2 size, vec<u8> data) {
	texture.updateTexture(pos.x, pos.y, size.x, size.y, data.data());
	
	for (u32 i = 0; i < size.x * size.y * 4; i++) {
		u32 x = (i / 4) % size.x;
		u32 y = (i / 4) / size.x;
		u32 of = i % 4;
		
		atlasData[(pos.x + x + (pos.y + y) * canvasSize.x) * 4 + of] = data[(x + y * size.x) * 4 + of];
	}
}

void TextureAtlas::deleteImage(sptr<AtlasRef> ref) {
	// Actually delete the image from the texture (for debugging)
	
	//auto data = new unsigned char[ref->width * ref->height * 4];
	//
	//for (int i = 0; i < ref->width * ref->height * 4; i++) {
	//    data[i] = 0;
	//}
	//
	//updateAtlas(ref->tileX, ref->tileY, ref->width, ref->height, data);
	//delete[] data;
	
	textureSlotsUsed -= ref->size.x * ref->size.y;
	
	for (u32 x = ref->pos.x; x < ref->pos.x + ref->size.x; x++) {
		for (u32 y = ref->pos.y; y < ref->pos.y + ref->size.y; y++) {
			empty[y * canvasTileSize.x + x] = true;
		}
	}
}