#include "TextureAtlas.h"
#include "TextureBuilder.h"
#include "game/atlas/asset/AtlasTexture.h"

TextureBuilder::Data TextureBuilder::Data::literal(Context& ctx, string str) {
	let tex = ctx.atlas.get(str);
	return Data(tex.getSize(), tex.getBytes());
}

TextureBuilder::Data TextureBuilder::Data::canvas(u16 w, optional<variant<u16, string>> h, optional<string> fill) {
	u16vec2 size = { w, h && std::holds_alternative<u16>(*h) ? std::get<u16>(*h) : w };
	vec4 color = Util::hexToColorVec(fill ? *fill : h &&
		std::holds_alternative<string>(*h) ? std::get<string>(*h) : "#0000");
	
	vec<u8> data(size.x * size.y * 4);
	for (u32 i = 0; i < size.x * size.y * 4; i++) data[i] = color[i % 4] * 255;
	return Data(size, data);
}

TextureBuilder::Data TextureBuilder::Data::alpha(f32 factor, Data tex) {
	for (u32 i = 0; i < tex.size.x * tex.size.y; i++)
		tex.data[i * 4 + 3] = (tex.data[i * 4 + 3] / 255.f * factor) * 255;
	return tex;
}

TextureBuilder::Data TextureBuilder::Data::crop(u16 x, u16 y, u16 w, variant<u16, Data> hV, optional<Data> texO) {
	u16 h = std::holds_alternative<u16>(hV) ? std::get<u16>(hV) : w;
	Data& tex = std::holds_alternative<Data>(hV) ? std::get<Data>(hV) : *texO;
	
	vec<u8> data(w * h * 4);
	for (u32 i = 0; i < w * h * 4; i++) data[i] =
		tex.data[((x + i / 4 % w) + (y + i / 4 / w) * tex.size.x) * 4 + i % 4];
	tex.data = data;
	
	if (tex.tintMask) {
		vec<u8> mask(w * h * 4);
		for (u32 i = 0; i < w * h * 4; i++) mask[i] =
			(*tex.tintMask)[((x + i / 4 % w) + (y + i / 4 / w) * tex.size.x) * 4 + i % 4];
		tex.tintMask = mask;
	}
	
	tex.size = { w, h };
	return tex;
}

TextureBuilder::Data TextureBuilder::Data::multiply(variant<string, Data> multiple, Data tex) {
	if (std::holds_alternative<Data>(multiple)) {
		Data& data = std::get<Data>(multiple);
		for (u32 i = 0; i < tex.size.x * tex.size.y * 4; i++) tex.data[i] *= data.data[i % 4];
	}
	else {
		vec4 mul = Util::hexToColorVec(std::get<string>(multiple));
		for (u32 i = 0; i < tex.size.x * tex.size.y * 4; i++) tex.data[i] *= mul[i % 4];
	}
	
	return tex;
}

TextureBuilder::Data TextureBuilder::Data::stack(Data b, Data s1, optional<Data> s2, optional<Data> s3,
	optional<Data> s4, optional<Data> s5, optional<Data> s6, optional<Data> s7, optional<Data> s8) {
	vec<Data> stack;
	stack.reserve(8);
	stack.emplace_back(s1);
	if (s2) stack.emplace_back(*s2);
	if (s3) stack.emplace_back(*s3);
	if (s4) stack.emplace_back(*s4);
	if (s5) stack.emplace_back(*s5);
	if (s6) stack.emplace_back(*s6);
	if (s7) stack.emplace_back(*s7);
	if (s8) stack.emplace_back(*s8);

	for (const Data& s : stack) {
		for (u32 i = 0; i < b.size.x * b.size.y; i++) {
			const f32 factor = s.data[i * 4 + 3] / 255.f;
			b.data[i * 4] = b.data[i * 4] * (1 - factor) + s.data[i * 4] * factor;
			b.data[i * 4 + 1] = b.data[i * 4 + 1] * (1 - factor) + s.data[i * 4 + 1] * factor;
			b.data[i * 4 + 2] = b.data[i * 4 + 2] * (1 - factor) + s.data[i * 4 + 2] * factor;
			b.data[i * 4 + 3] = std::min(static_cast<u16>(b.data[i * 4 + 3]) + s.data[i * 4 + 3], 255);
			
			if (s.tintMask) {
				if (!b.tintMask) b.tintMask = canvas(b.size.x, b.size.y, {}).data;
				(*b.tintMask)[i * 4] = (*b.tintMask)[i * 4] * (1 - factor) + (*s.tintMask)[i * 4] * factor;
				(*b.tintMask)[i * 4 + 1] = (*b.tintMask)[i * 4 + 1] * (1 - factor) + (*s.tintMask)[i * 4 + 1] * factor;
				(*b.tintMask)[i * 4 + 2] = (*b.tintMask)[i * 4 + 2] * (1 - factor) + (*s.tintMask)[i * 4 + 2] * factor;
				(*b.tintMask)[i * 4 + 3] = factor * 255;
			}
		}
	}

	return b;
}

TextureBuilder::Data TextureBuilder::Data::tint(u32 tint, TextureBuilder::Data tex, optional<Data> mask) {
	tex.tintInd = tint;
	if (mask) tex.tintMask = mask->data;
	else tex.tintMask = canvas(tex.size.x, tex.size.y, "#fff").data;
	return tex;
}

TextureBuilder::TextureBuilder(TextureAtlas& atlas): ctx({ atlas }) {
	parser.addLiteralFnCtx(&TextureBuilder::Data::literal);
	parser.addFn<u16, optional<variant<u16, string>>, optional<string>>("canvas", &TextureBuilder::Data::canvas);
	parser.addFn<f32, Data>("alpha", &TextureBuilder::Data::alpha);
	parser.addFn<u16, u16, u16, variant<u16, Data>, optional<Data>>("crop", &TextureBuilder::Data::crop);
	parser.addFn<variant<string, Data>, Data>("multiply", &TextureBuilder::Data::multiply);
	parser.addFn<Data, Data, optional<Data>, optional<Data>, optional<Data>, optional<Data>,
		optional<Data>, optional<Data>, optional<Data>>("", &TextureBuilder::Data::stack);
	parser.addFn<u32, Data, optional<Data>>("tint", &TextureBuilder::Data::tint);
}

TextureBuilder::Data TextureBuilder::build(const string& str) const {
	return parser.parse(str, ctx);
}