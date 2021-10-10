#pragma once

#include "util/StringParser.h"
//#include "game/atlas/TextureAtlas.h"

class TextureAtlas;
class AtlasTexture;

class TextureBuilder {
public:
	struct Data;

private:
	struct Context {
		TextureAtlas& atlas;
	};
	
	typedef StringParser<Data, Context> Parser;

public:
	class Texture {
		struct ByteData {
			ByteData() = default;
			ByteData(const vec<u8>& data): data(data) {};
			
			vec<u8> data;
		};
		
		struct CropData {
			CropData(u16vec2 offset, const sptr<AtlasTexture>& source): offset(offset), source(source) {}
			
			u16vec2 offset;
			sptr<AtlasTexture> source;
		};
		
		vec<u8>& getBytes();
		
	public:
		void alpha(f32 factor);
		
		void crop(u16vec2 pos, u16vec2 size);
		
		void multiply(vec4 scalar);
		
		void multiply(Texture& texture);
		
		void stack(Texture& texture);
		
		Texture(u16vec2 size, vec4 color):
			size(size), data(ByteData(vec<u8>(size.x * size.y * 4))) {
			let& bytes = get<ByteData>(data).data;
			for (u32 i = 0; i < size.x * size.y * 4; i++) bytes[i] = color[i % 4] * 255;
		}
		
		Texture(u16vec2 size, sptr<AtlasTexture> tex):
			size(size), data(CropData({}, tex)) {}
			
		Texture(u16vec2 pos, u16vec2 size, sptr<AtlasTexture> tex):
			size(size), data(CropData(pos, tex)) {}
		
		u16vec2 size;
		variant<CropData, ByteData> data;
	};
	
	class Data {
		friend class TextureBuilder;
		
		/**
		 * Loads a texture identified by a string into a Data.
		 *
		 * @param str - The string texture name to load.
		 */
		 
		static Parser::Data literal(Parser::Ctx& ctx, string str);

		/**
		 * Creates an empty canvas of a specified color.
		 *
		 * @param w - The width of the canvas.
		 * @param h - [optional] The height of the canvas, defaults to the width otherwise.
		 * @param fill - [optional] The color to fill the canvas with, defaults to transparent.
		 */
		 
		static Parser::Data canvas(u16 w, optional<variant<u16, string>> h, optional<string> fill);
		
		/**
		 * Adjusts the opacity of the provided texture by a factor.
		 *
		 * @param factor - The factor that the opacity should be scaled by. 0 = transparent, 1 = opaque.
		 * @param tex - The texture to scale the alpha of.
		 */
		 
		static Parser::Data alpha(f32 factor, Parser::Data tex);
		
		/**
		 * Crops the texture to the specified dimensions and offset.
		 *
		 * @param x - The left edge of the crop on the base texture.
		 * @param y - The top edge of the crop on the base texture.
		 * @param w - The width of the crop.
		 * @param h - The height of the crop.
		 * @param tex - The texture to crop.
		 */
	
		static Parser::Data crop(u16 x, u16 y, u16 w, variant<u16, Data> hV, optional<Data> texO);

		/**
		 * Multiplies the texture by the specified color or texture.
		 *
		 * @param multiple - A color to multiply the texture by, or another texture to multiply it by.
		 * @param tex - The texture to multiply.
		 */
		 
		static Parser::Data multiply(variant<string, Data> multiple, Data tex);

		/**
		 * Stacks the provided textures on top of each other, blending partially opaque pixels.
		 * Requires at least two textures. The maximum is currently eight textures,
		 * however that could be removed later with some tweaks to StringParser.
		 *
		 * @param b - The base texture to stack onto.
		 * @param s1 - A texture to stack on top of the base texture.
		 * @param s2 - Another texture to stack on top of s1.
		 * @param s3 - ...
		 */

		static Parser::Data stack(Data b, Data s1, optional<Data> s2, optional<Data> s3,
			optional<Data> s4, optional<Data> s5, optional<Data> s6, optional<Data> s7, optional<Data> s8);
		
		/**
		 * Tints the texture specified with a specific tint map, usually controlled by biome parameters.
		 * @param tint - The tint map to use.
		 * @param tex - The texture to tint.
		 * @param mask - An image whose red channel determines the strength of the tint for every pixel.
		 */
		
		static Parser::Data tint(u32 tint, Data tex, optional<Data> mask);
	
	public:
		Data() = default;
		
		Data(Data&) = default;
		Data(Data&&) = default;
		Data& operator=(Data&) = default;
		Data& operator=(Data&&) = default;
		
		Data(u16vec2 size, vec4 color): texture(make_unique<Texture>(size, color)) {};
		Data(u16vec2 size, sptr<AtlasTexture> tex): texture(make_unique<Texture>(size, tex)) {};
		Data(u16vec2 pos, u16vec2 size, sptr<AtlasTexture> tex): texture(make_unique<Texture>(pos, size, tex)) {};
		
//		Data(Data& o): texture(std::move(o.texture)) {};
//		Data(Data&& o): texture(std::move(o.texture)) {};
//		Data& operator=(Data& o) { this->texture = std::move(o.texture); return *this; };
//		Data& operator=(Data&& o) { this->texture = std::move(o.texture); return *this; };
		
		sptr<Texture> texture = {};
		optional<std::pair<u32, sptr<Texture>>> tintMask = {};
	};
	
	TextureBuilder(TextureAtlas& atlas);
	
	Data build(const string& str) const;

private:
	
	mutable Context ctx;
	Parser parser {};
};


