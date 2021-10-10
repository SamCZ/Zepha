#include "TextureAtlas.h"
#include "TextureBuilder.h"
#include "game/atlas/asset/AtlasTexture.h"

vec<u8>& TextureBuilder::Texture::getBytes() {
	if (std::holds_alternative<ByteData>(data)) return get<ByteData>(data).data;
	const CropData& cropData = get<CropData>(data);
	const vec<u8> sourceBytes = cropData.source->getBytes();
	const u16 sourceWidth = cropData.source->getSize().x;
	
	vec<u8> newBytes(size.x * size.y * 4);
	for (u32 i = 0; i < size.x * size.y * 4; i++) newBytes[i] =
		sourceBytes[((cropData.offset.x + i / 4 % size.x) +
		(cropData.offset.y + i / 4 / size.x) * sourceWidth) * 4 + i % 4];
	
	data = ByteData(newBytes);
	return get<ByteData>(data).data;
}

void TextureBuilder::Texture::alpha(f32 factor) {
	let& data = getBytes();
	for (u32 i = 0; i < size.x * size.y; i++)
		data[i * 4 + 3] = (data[i * 4 + 3] / 255.f * factor) * 255;
}

void TextureBuilder::Texture::crop(u16vec2 pos, u16vec2 size) {
	if (std::holds_alternative<CropData>(data)) {
		CropData& crop = get<CropData>(data);
		crop.offset += pos;
		this->size = size;
	}
	else {
		const let& data = getBytes();
		vec<u8> newData(size.x * size.y * 4);
		for (u32 i = 0; i < size.x * size.y * 4; i++) newData[i] =
			data[((pos.x + i / 4 % size.x) + (pos.y + i / 4 / size.x) * this->size.x) * 4 + i % 4];
		this->data = ByteData(newData);
		this->size = size;
	}
}

void TextureBuilder::Texture::multiply(vec4 scalar) {
	let& data = getBytes();
	for (u32 i = 0; i < size.x * size.y * 4; i++) data[i] *= scalar[i % 4];
}

void TextureBuilder::Texture::multiply(Texture& texture) {
	let& data = getBytes();
	let& mul = texture.getBytes();
	for (u32 i = 0; i < size.x * size.y * 4; i++) data[i] *= mul[i];
}

void TextureBuilder::Texture::stack(Texture& texture) {
	let& data = getBytes();
	for (u32 i = 0; i < size.x * size.y; i++) {
		const let& oData = texture.getBytes();
		const f32 factor = oData[i * 4 + 3] / 255.f;
		data[i * 4] = data[i * 4] * (1 - factor) + oData[i * 4] * factor;
		data[i * 4 + 1] = data[i * 4 + 1] * (1 - factor) + oData[i * 4 + 1] * factor;
		data[i * 4 + 2] = data[i * 4 + 2] * (1 - factor) + oData[i * 4 + 2] * factor;
		data[i * 4 + 3] = std::min(static_cast<u16>(data[i * 4 + 3]) + oData[i * 4 + 3], 255);
	}
}


TextureBuilder::Data TextureBuilder::Data::literal(Context& ctx, string str) {
	let tex = ctx.atlas.get(str);
	return Data(tex->getSize(), {}, tex);
}

TextureBuilder::Data TextureBuilder::Data::canvas(u16 w, optional<variant<u16, string>> h, optional<string> fill) {
	u16vec2 size = { w, h && std::holds_alternative<u16>(*h) ? std::get<u16>(*h) : w };
	vec4 color = Util::hexToColorVec(fill ? *fill : h &&
		std::holds_alternative<string>(*h) ? std::get<string>(*h) : "#0000");
	return Data(size, color);
}

TextureBuilder::Data TextureBuilder::Data::alpha(f32 factor, Data data) {
	data.texture->alpha(factor);
	return data;
}

TextureBuilder::Data TextureBuilder::Data::crop(u16 x, u16 y, u16 w, variant<u16, Data> hV, optional<Data> texO) {
	u16 h = std::holds_alternative<u16>(hV) ? std::get<u16>(hV) : w;
	Data data = std::holds_alternative<Data>(hV) ? std::get<Data>(hV) : *texO;
	
	data.texture->crop({ x, y }, { w, h });
	return data;
}

TextureBuilder::Data TextureBuilder::Data::multiply(variant<string, Data> multiple, Data data) {
	if (std::holds_alternative<Data>(multiple)) data.texture->multiply(*get<Data>(multiple).texture.get());
	else data.texture->multiply(Util::hexToColorVec(get<string>(multiple)));
	return data;
}

TextureBuilder::Data TextureBuilder::Data::stack(Data b, Data s1, optional<Data> s2, optional<Data> s3,
	optional<Data> s4, optional<Data> s5, optional<Data> s6, optional<Data> s7, optional<Data> s8) {
	
	vec<Data*> stack;
	stack.reserve(8);
	stack.emplace_back(&s1);
	if (s2) stack.emplace_back(&*s2);
	if (s3) stack.emplace_back(&*s3);
	if (s4) stack.emplace_back(&*s4);
	if (s5) stack.emplace_back(&*s5);
	if (s6) stack.emplace_back(&*s6);
	if (s7) stack.emplace_back(&*s7);
	if (s8) stack.emplace_back(&*s8);

	for (const Data* s : stack) b.texture->stack(*s->texture.get());
	return b;
}

TextureBuilder::Data TextureBuilder::Data::tint(u32 tint, Data tex, optional<Data> mask) {
	if (mask) tex.tintMask = { tint, std::move(mask->texture) };
	else tex.tintMask = { tint, make_unique<Texture>(tex.texture->size, vec4(1)) };
	return tex;
}

TextureBuilder::TextureBuilder(TextureAtlas& atlas): ctx({ atlas }) {
	parser.addLiteralFnCtx(&TextureBuilder::Data::literal);
	parser.addFn<u16, optional<variant<u16, string>>, optional<string>>("canvas", &TextureBuilder::Data::canvas);
	parser.addFn<f32, Data>("alpha", &TextureBuilder::Data::alpha);
	parser.addFn<u16, u16, u16, variant<u16, Data>>("crop", &TextureBuilder::Data::crop);
	parser.addFn<u16, u16, u16, u16, Data>("crop", &TextureBuilder::Data::crop);
	parser.addFn<variant<string, Data>, Data>("multiply", &TextureBuilder::Data::multiply);
	parser.addFn<Data, Data, optional<Data>, optional<Data>, optional<Data>, optional<Data>,
		optional<Data>, optional<Data>, optional<Data>>("", &TextureBuilder::Data::stack);
	parser.addFn<u32, Data, optional<Data>>("tint", &TextureBuilder::Data::tint);
}

TextureBuilder::Data TextureBuilder::build(const string& str) const {
	return parser.parse(str, ctx);
}