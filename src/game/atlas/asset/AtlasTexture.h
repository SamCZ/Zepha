#pragma once

#include "util/Util.h"

class TextureAtlas;

/**
 * Represents a single drawable texture from the texture atlas.
 * Will either contain its own data, or reference another atlas texture as a crop of it.
 * AtlasTextures will automatically remove themselves from the texture atlas once all copies of them are destroyed.
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
	
	/**
	 * The main data container for the AtlasTexture. It is stored in a
	 * shared pointer so that it may be reference counted.
	 * Contains the identifier, atlas position, and data of a texture in the atlas.
	 * The data may be a ByteData storing raw texture bytes,
	 * or a CropData referencing another texture to crop this one from.
	 * May also store a reference to a tint index and mask, if the texture is to be biome tinted.
	 */
	 
	struct SharedData {
		SharedData(const string& identifier, uvec2 pos, const variant<ByteData, CropData>& data):
			identifier(identifier), pos(pos), data(data) {}
		
		/** The position of the top-left point of the texture in the texture atlas. */
		uvec2 pos {};
		
		/** The identifier of the texture. */
		string identifier;
		
		/** Whether or not the texture should be kept even if it is not in use. */
		bool persistent = false;
		
		/** The data of the texture, either bytes or a crop region. */
		variant<ByteData, CropData> data;
		
		/** An optional pair containing biome tint data. */
		optional<std::pair<u32, uptr<AtlasTexture>>> tintData;
	};

public:
	AtlasTexture(const AtlasTexture& o): shr(o.shr), atlas(o.atlas) {}
	
	AtlasTexture& operator=(const AtlasTexture& o) { shr = o.shr; return *this; }
	
	AtlasTexture(TextureAtlas& atlas, const string& identifier, uvec2 pos, uvec2 size, const sptr<AtlasTexture>& source):
		AtlasTexture(atlas, identifier, source->getPos(), CropData { pos, size, source }) {}
		
	AtlasTexture(TextureAtlas& atlas, const string& identifier, uvec2 pos, uvec2 size, vec<u8> data):
		AtlasTexture(atlas, identifier, pos, ByteData { size, data }) {}
		
	AtlasTexture(TextureAtlas& atlas, const string& identifier, uvec2 pos, const variant<ByteData, CropData>& data):
		shr(std::make_shared<SharedData>(identifier, pos, data)), atlas(atlas) {}
	
	/** Returns the texture's identifier. */
	inline const string& getIdentifier() const {
		return shr->identifier;
	}
	
	/** Returns the texture's atlas position in pixels. */
	inline const uvec2& getPos() const {
		return shr->pos;
	}
	
	/** Returns the texture's altas size in pixels. */
	inline const uvec2& getSize() const {
		return holds_alternative<ByteData>(shr->data) ?
			get<ByteData>(shr->data).size : get<CropData>(shr->data).size;
	}
	
	/** Returns the texture's atlas position in tiles. */
	inline u16vec2 getTilePos() const {
		return u16vec2(shr->pos / 16u);
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
	optional<AtlasTexture> getTintMask() const;
	
	/** Sets the persistence of the texture, if it is persistent, it will not be deleted when it is no longer in use. */
	void setPersistent(bool persistent);
	
	/** Sets the tint information of the texture. */
	void setTintData(u32 tintInd, const AtlasTexture& tintMask);
	
	/** Returns true if the texture is only referenced by the texture atlas and is not persistent. */
	bool shouldBeRemoved();
	
	~AtlasTexture();

private:
	
	// IF YOU ARE PUTTING SOMETHING ELSE HERE, YOU PROBABLY SHOULDN'T BE.
	// PUT ANY ATLASTEXTURE DATA IN THE SHAREDDATA STRUCT INSTEAD.
	
	/** A reference to the texture atlas holding this texture. */
	TextureAtlas& atlas;
	
	/** The actual ref-counted data of the AtlasTexture. */
	sptr<SharedData> shr;
};