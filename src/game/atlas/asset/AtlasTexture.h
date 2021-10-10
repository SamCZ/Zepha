#pragma once

#include "util/Util.h"

class TextureAtlas;

/**
 * Represents a single drawable texture from the texture atlas.
 * Will either contain its own data, or reference another atlas texture as a crop of it.
 * To be stored in a shared pointer and kept by dependents in AtlasRef, generated by AtlasTexture.
 */

class AtlasTexture {
	
	/** Stores byte data for an atlas texture. */
	struct ByteData {
		ByteData(uvec2 size, const vec<u8>& data): data(data), size(size) {};
		
		/** The size of the texture. */
		uvec2 size;
		
		/** The raw bytes of the texture. */
		vec<u8> data;
	};
	
	/** Stores a reference to another atlas texture and a crop region. */
	struct CropData {
		CropData(uvec2 pos, uvec2 size, const sptr<AtlasTexture>& source): pos(pos), size(size), source(source) {}
		
		/** The top-left position of the crop region. */
		uvec2 pos;
		
		/** The sise of the crop region. */
		uvec2 size;
		
		/** The image to crop from. */
		sptr<AtlasTexture> source;
	};

public:
	
	AtlasTexture(const AtlasTexture& o) = delete;
	
	AtlasTexture(TextureAtlas& atlas, const string& identifier, uvec2 pos, uvec2 size, const sptr<AtlasTexture>& source):
		AtlasTexture(atlas, identifier, source->getPos(), CropData { pos, size, source }) {}
		
	AtlasTexture(TextureAtlas& atlas, const string& identifier, uvec2 pos, uvec2 size, vec<u8> data):
		AtlasTexture(atlas, identifier, pos, ByteData { size, data }) {}
		
	AtlasTexture(TextureAtlas& atlas, const string& identifier, uvec2 pos, const variant<ByteData, CropData>& data):
		identifier(identifier), pos(pos), data(data), atlas(atlas) {}
	
	/** Returns the texture's identifier. */
	inline const string& getIdentifier() const {
		return identifier;
	}
	
	/** Returns the texture's atlas position in pixels. */
	inline const uvec2& getPos() const {
		return pos;
	}
	
	/** Returns the texture's altas size in pixels. */
	inline const uvec2& getSize() const {
		return holds_alternative<ByteData>(data) ? get<ByteData>(data).size : get<CropData>(data).size;
	}
	
	/** Returns the texture's atlas position in tiles. */
	inline u16vec2 getTilePos() const {
		return u16vec2(pos / 16u);
	}

	/** Returns the texture's atlas size in tiles. */
	inline u16vec2 getTileSize() const {
		return u16vec2(glm::ceil(vec2(getSize()) / 16.f));
	}

	/** Returns the texture's UVs, in a four-dimensional vector. */
	vec4 getUVPos() const;
	
	/**
	 * Returns the bytes of the texture.
	 * This is an expensive operation because the bytes may need to be cloned from a crop region,
	 * so a reference cannot be returned. In the future,
	 * a ref-counted object may be returned instead to reduce overhead.
	 */
	 
	vec<u8> getBytes() const;
	
	/** Returns the biome tint index of the texture, if it is to be tinted. */
	optional<u32> getTintInd() const;
	
	/** Returns the biome tint mask texture of this texture, if it is to be tinted. */
	optional<sptr<AtlasTexture>> getTintMask() const;
	
	/** Sets the tint information of the texture. */
	void setTintData(u32 tintInd, const sptr<AtlasTexture>& tintMask);

private:

	/** The position of the top-left point of the texture in the texture atlas. */
	uvec2 pos {};
	
	/** The identifier of the texture. */
	string identifier;
	
	/** The data of the texture, either bytes or a crop region. */
	variant<ByteData, CropData> data;
	
	/** An optional pair containing biome tint data. */
	optional<std::pair<u32, sptr<AtlasTexture>>> tintData;

	/** A reference to the texture atlas holding this texture. */
	TextureAtlas& atlas;
};

using AtlasRef = sptr<AtlasTexture>;