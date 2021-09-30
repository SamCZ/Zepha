#include <functional>

#include "TextElement.h"

#include "client/gui/Root.h"
#include "client/graph/Model.h"
#include "client/graph/mesh/EntityMesh.h"

void Gui::TextElement::updateElement() {
	const string text = getStyle<string>(Prop::CONTENT, "");
	
	if (!font) font = make_unique<Font>(root.atlas["font"]);
	
	usize newHash = std::hash<string>{}(text);
	if (hash != newHash) {
		hash = newHash;
		
		vec4 textColor = getStyle<vec4, Type::COLOR>(Prop::TEXT_COLOR, vec4(1));
		vec4 backgroundColor = getStyle<vec4, Type::COLOR>(Prop::BACKGROUND, vec4(0));
		
		u32 ind = 0;
		u32 width = 0;

		vec<EntityVertex> vertices;
		vertices.reserve(text.length() * 8 + 200);
		vec<u32> indices;
		indices.reserve(text.length() * 12 + 240);

		vec<string> lines;
		{
			std::stringstream textStream(text);
			string line;
			while (std::getline(textStream, line, '\n')) lines.emplace_back(line);
		}

		vec3 offset = {};
		u32 h = Font::C_HEIGHT;

		bool bold = false;
		bool italic = false;
		i32 underline = -1;
		i32 strikethrough = -1;
		u32 strikethroughVertStart = 0;
		vec4 color = textColor;

		for (usize i = 0; i < lines.size(); i++) {
			let& line = lines[i];
			bool empty = line.find_first_not_of(" \t\n") == -1;

			if (empty) {
				offset.x = 0;
				offset.y += h / 2;
				continue;
			}

			u32 bgVertStart = 0;
			if (backgroundColor.w != 0) {
				bgVertStart = vertices.size();
				for (u32 i = 0; i < 4; i++) vertices.push_back({});
				for (u32 i : INDICES) indices.push_back(i + ind);
				ind += 4;
			}

			for (usize j = 0; j < line.length() + 1; j++) {
				char c = j < line.length() ? line[j] : ' ';
				if (c == '\t') c = ' ';

				if (c == '`') {
					bool flushDecorators = j == line.length();

					char d = line[++j];
					if (d == '`') goto escape_formatting;
					else if (d == ' ') offset.x++;
					else if (d == 'b') bold = true;
					else if (d == 'i') italic = true;
					else if (d == 'u') underline = offset.x;
					else if (d == 's') {
						strikethrough = offset.x;
						strikethroughVertStart = vertices.size();
						for (u32 i = 0; i < 4; i++) vertices.push_back({});
						for (u32 i : INDICES) indices.push_back(i + ind);
						ind += 4;
					}
					else if (d == 'c') flushDecorators = true;
					else if (d == 'r') {
						bold = false;
						italic = false;
						flushDecorators = true;
					}

					if (flushDecorators) {
						if (underline != -1) {
							TextElement::drawRect(
								{ underline, offset.y + h - 1, offset.x, offset.y + h },
								color, vertices, indices, ind);
							TextElement::drawRect(
								{ underline + 1, offset.y + h, offset.x + 1, offset.y + h + 1 },
								color * BG_MULTIPLE, vertices, indices, ind);
							underline = offset.x;
						}

						if (strikethrough != -1) {
							TextElement::drawRect(
								{ strikethrough, offset.y + h / 2, offset.x, offset.y + h / 2 + 1 },
								color, vertices, indices, ind);
							TextElement::drawRect(
								{ strikethrough + 1, offset.y + h / 2 + 1, offset.x + 1, offset.y + h / 2 + 2 },
								color * BG_MULTIPLE, vertices, indices, ind, strikethroughVertStart);
							strikethrough = offset.x;
						}

						if (d == 'r') {
							color = textColor;
							underline = -1;
							strikethrough = -1;
						}
					}

					if (d == 'c') {
						char code = line[++j];
						if (code == 'r') color = textColor;
						else {
							u32 v;
							std::stringstream ss;
							ss << std::hex << code;
							ss >> v;
							color = COLORS[v];
						}
					}

					continue;
				}

				escape_formatting:
				if (j == line.length()) continue;

				u32 w = font->getCharWidth(c) + 1;
				vec4 UV = font->getCharUVs(c);

				for (u32 k = 0; k < (bold ? 4 : 2); k++) {
					vec4 c = color;

					if (k == 0 || (k == 1 && bold)) c *= BG_MULTIPLE;

					if (k == 0) {
						offset.x += 1;
						offset.y += 1;
					}
					else if ((k == 1 || k == 3) && bold) {
						offset.x += 1;
					}
					else if ((k == 1 && !bold) || (k == 2 && bold)) {
						offset.x -= bold ? 2 : 1;
						offset.y -= 1;
					}

					vertices.emplace_back(offset + vec3(italic ? 2 : 0, 0, 0), vec4 { UV.x, UV.y, 0, c.w },
						vec3(c), 1.f, vec3 {}, ivec4 {}, vec4 {});

					vertices.emplace_back(offset + vec3(0, h, 0), vec4 { UV.x, UV.w, 0, c.w },
						vec3(c), 1.f, vec3 {}, ivec4 {}, vec4 {});

					vertices.emplace_back(offset + vec3(w, h, 0), vec4 { UV.z, UV.w, 0, c.w },
						vec3(c), 1.f, vec3 {}, ivec4 {}, vec4 {});

					vertices.emplace_back(offset + vec3(w + (italic ? 2 : 0), 0, 0), vec4 { UV.z, UV.y, 0, c.w },
						vec3(c), 1.f, vec3 {}, ivec4 {}, vec4 {});

					for (u32 i : INDICES) indices.push_back(i + ind);
					ind += 4;
				}

				offset.x += w;
			}

			if (backgroundColor.w != 0) TextElement::drawRect({ -1, offset.y - 1, offset.x + 2, offset.y + h + 1 },
				backgroundColor, vertices, indices, ind, bgVertStart);

			if (offset.x > width) width = offset.x;
			offset.x = 0;
			offset.y += h + 2;
		}

		let mesh = make_unique<EntityMesh>();
		mesh->create(vertices, indices);

		let model = make_shared<Model>();
		model->fromMesh(std::move(mesh));
		entity.setModel(model);
		
	}
	
	let scale = getStyle<f32, Type::LENGTH>(Prop::TEXT_SIZE, 3.f);
	let margin = getStyle<vec4, Type::LENGTH>(Prop::MARGIN, {});
	
	entity.setScale(ivec3(scale, scale, 0));
	entity.setPos(ivec3(getComputedScreenPos() + vec2 { margin.x, margin.y }, 0));
	
	Element::updateElement();
}

void Gui::TextElement::drawRect(const vec4 pos, const vec4 color,
	vec<EntityVertex>& vertices, vec<u32>& indices, u32& ind, const u32 insert) {

	vec<EntityVertex> myVerts = {
		{ vec3 { pos.x, pos.y, 0 }, color, vec3(1), 0.f, vec3 {}, ivec4 {}, vec4 {} },
		{ vec3 { pos.x, pos.w, 0 }, color, vec3(1), 0.f, vec3 {}, ivec4 {}, vec4 {} },
		{ vec3 { pos.z, pos.w, 0 }, color, vec3(1), 0.f, vec3 {}, ivec4 {}, vec4 {} },
		{ vec3 { pos.z, pos.y, 0 }, color, vec3(1), 0.f, vec3 {}, ivec4 {}, vec4 {} }
	};

	if (insert != -1) {
		vertices[insert] = myVerts[0];
		vertices[insert + 1] = myVerts[1];
		vertices[insert + 2] = myVerts[2];
		vertices[insert + 3] = myVerts[3];
	}
	else {
		for (EntityVertex& vert : myVerts) vertices.emplace_back(vert);
		for (u32 i : INDICES) indices.push_back(i + ind);
		ind += 4;
	}
}

const array<vec4, 16> Gui::TextElement::COLORS = {
	Util::hexToColorVec("#ffffff"),
	Util::hexToColorVec("#aaaaaa"),
	Util::hexToColorVec("#666666"),
	Util::hexToColorVec("#000000"),
	
	Util::hexToColorVec("#f53658"),
	Util::hexToColorVec("#ff9940"),
	Util::hexToColorVec("#fffb82"),
	Util::hexToColorVec("#9fff80"),
	Util::hexToColorVec("#0fa84f"),
	Util::hexToColorVec("#26d4d4"),
	Util::hexToColorVec("#7df4ff"),
	Util::hexToColorVec("#33a2f5"),
	Util::hexToColorVec("#2c58e8"),
	Util::hexToColorVec("#b05cff"),
	Util::hexToColorVec("#fd7dff"),
	Util::hexToColorVec("#ff739f"),
};

const array<u32, 6> Gui::TextElement::INDICES = { 0, 1, 2, 2, 3, 0 };

const vec4 Gui::TextElement::BG_MULTIPLE = { 0.3, 0.3, 0.35, 0.75 };