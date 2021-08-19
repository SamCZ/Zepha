//#include "GuiText.h"
//
//#include "util/Util.h"
//#include "client/gui/SerialGui.h"
//#include "game/atlas/TextureAtlas.h"
//#include "game/atlas/asset/AtlasRef.h"
//#include "world/dim/ent/AnimationSegment.h"
//
//void GuiText::create(vec2 scale, vec4 padding, vec4 textColor, vec4 backgroundColor, Font font) {
//	this->padding = padding;
//	this->font = std::move(font);
//
//	this->textColor = textColor;
//	this->backgroundColor = backgroundColor;
//
//	setText("");
//	setScale(scale);
//}
//
//sptr<GuiText> GuiText::fromSerialized(const LuaGuiElement& elem, Window& window, TextureAtlas& textures, ivec2 bounds) {
//	vec2 pos     = SerialGui::get<vec2>(elem, "position", bounds);
//	vec2 offset  = SerialGui::get<vec2>(elem, "position_anchor");
//	vec2 size    = SerialGui::get<vec2>(elem, "size", bounds);
//	vec4 padding = SerialGui::get<vec4>(elem, "padding", bounds);
//	vec2 scale   = SerialGui::get<vec2>(elem, "scale") * vec2(0.33);
//
//	if (scale.x == 0 && scale.y == 0) scale = vec2 { 1, 1 };
//	pos -= offset * size;
//
//	vec4 backgroundColor = Util::hexToColorVec(elem.get_or<string>("background", "#0000"));
//	vec4 textColor = Util::hexToColorVec(elem.get_or<string>("color", "#fff"));
//	string content = elem.get_or<string>("content", "");
//
//	auto text = std::make_shared<GuiText>(window, elem.key);
//	text->create(scale * SerialGui::SCALE_MODIFIER, padding,
//		textColor, backgroundColor, { textures, textures["font"] });
//	text->setText(content);
//	text->setPos(pos);
//
//	return text;
//}
//
//void GuiText::setText(string newText) {
//	text = newText;
//	u32 ind = 0;
//
//	width = 0;
//
//	vec<EntityVertex> vertices;
//	vertices.reserve(text.length() * 8 + 200);
//	vec<u32> indices;
//	indices.reserve(text.length() * 12 + 240);
//
//	vec<string> lines;
//	{
//		std::stringstream textStream(text);
//		string line;
//		while (std::getline(textStream, line, '\n')) lines.emplace_back(line);
//	}
//
//	vec3 offset = {};
//	u32 h = Font::charHeight;
//
//	bool bold = false;
//	bool italic = false;
//	i32 underline = -1;
//	i32 strikethrough = -1;
//	u32 strikethroughVertStart = 0;
//	vec4 color = textColor;
//
//	for (usize i = 0; i < lines.size(); i++) {
//		let& line = lines[i];
//		bool empty = line.find_first_not_of(" \t\n") == -1;
//
//		if (empty) {
//			offset.x = 0;
//			offset.y += h / 2;
//			continue;
//		}
//
//		u32 bgVertStart = 0;
//		if (backgroundColor.w != 0) {
//			bgVertStart = vertices.size();
//			for (u32 i = 0; i < 4; i++) vertices.push_back({});
//			for (u32 i : INDICES) indices.push_back(i + ind);
//			ind += 4;
//		}
//
//		for (usize j = 0; j < line.length() + 1; j++) {
//			char c = j < line.length() ? line[j] : ' ';
//			if (c == '\t') c = ' ';
//
//			if (c == '`') {
//				bool flushDecorators = j == line.length();
//
//				char d = line[++j];
//				if (d == '`') goto escape_formatting;
//				else if (d == ' ') offset.x++;
//				else if (d == 'b') bold = true;
//				else if (d == 'i') italic = true;
//				else if (d == 'u') underline = offset.x;
//				else if (d == 's') {
//					strikethrough = offset.x;
//					strikethroughVertStart = vertices.size();
//					for (u32 i = 0; i < 4; i++) vertices.push_back({});
//					for (u32 i : INDICES) indices.push_back(i + ind);
//					ind += 4;
//				}
//				else if (d == 'c') flushDecorators = true;
//				else if (d == 'r') {
//					bold = false;
//					italic = false;
//					flushDecorators = true;
//				}
//
//				if (flushDecorators) {
//					if (underline != -1) {
//						GuiText::drawRect({ underline, h - 1, offset.x, h }, color, vertices, indices, ind);
//						GuiText::drawRect({ underline + 1, h, offset.x + 1, h + 1 },
//							color * BG_MULTIPLE, vertices, indices, ind);
//						underline = offset.x;
//					}
//
//					if (strikethrough != -1) {
//						GuiText::drawRect({ strikethrough, h / 2, offset.x, h / 2 + 1 },
//							color, vertices, indices, ind);
//						GuiText::drawRect({ strikethrough + 1, h / 2 + 1, offset.x + 1, h / 2 + 2 },
//							color * BG_MULTIPLE, vertices, indices, ind, strikethroughVertStart);
//						strikethrough = offset.x;
//					}
//
//					if (d == 'r') {
//						color = textColor;
//						underline = -1;
//						strikethrough = -1;
//					}
//				}
//
//				if (d == 'c') {
//					char code = line[++j];
//					if (code == 'r') color = textColor;
//					else {
//						u32 v;
//						std::stringstream ss;
//						ss << std::hex << code;
//						ss >> v;
//						color = COLORS[v];
//					}
//				}
//
//				continue;
//			}
//
//			escape_formatting:
//			if (j == line.length()) continue;
//
//			u32 w = font.getCharWidth(c) + 1;
//			vec4 UV = font.getCharUVs(c);
//
//			for (u32 k = 0; k < (bold ? 4 : 2); k++) {
//				vec4 c = color;
//
//				if (k == 0 || (k == 1 && bold)) c *= BG_MULTIPLE;
//
//				if (k == 0) {
//					offset.x += 1;
//					offset.y += 1;
//				}
//				else if ((k == 1 || k == 3) && bold) {
//					offset.x += 1;
//				}
//				else if ((k == 1 && !bold) || (k == 2 && bold)) {
//					offset.x -= bold ? 2 : 1;
//					offset.y -= 1;
//				}
//
//				vertices.emplace_back(offset + vec3(italic ? 2 : 0, 0, 0), vec4 { UV.x, UV.y, 0, c.w },
//					vec3(c), 1.f, vec3 {}, ivec4 {}, vec4 {});
//
//				vertices.emplace_back(offset + vec3(0, h, 0), vec4 { UV.x, UV.w, 0, c.w },
//					vec3(c), 1.f, vec3 {}, ivec4 {}, vec4 {});
//
//				vertices.emplace_back(offset + vec3(w, h, 0), vec4 { UV.z, UV.w, 0, c.w },
//					vec3(c), 1.f, vec3 {}, ivec4 {}, vec4 {});
//
//				vertices.emplace_back(offset + vec3(w + (italic ? 2 : 0), 0, 0), vec4 { UV.z, UV.y, 0, c.w },
//					vec3(c), 1.f, vec3 {}, ivec4 {}, vec4 {});
//
//				for (u32 i : INDICES) indices.push_back(i + ind);
//				ind += 4;
//			}
//
//			offset.x += w;
//		}
//
//		if (backgroundColor.w != 0) GuiText::drawRect({ -1, offset.y - 1, offset.x + 2, offset.y + h + 1 },
//			backgroundColor, vertices, indices, ind, bgVertStart);
//
//		if (offset.x > width) width = offset.x;
//		offset.x = 0;
//		offset.y += h + 2;
//	}
//
//	let m = make_unique<EntityMesh>();
//	m->create(vertices, indices);
//
//	let model = make_shared<Model>();
//	model->fromMesh(std::move(m));
//
//	entity.setModel(model);
//}
//
//string GuiText::getText() {
//	return text;
//}
//
//u32 GuiText::getWidth() {
//	return width;
//}
//
//void GuiText::drawRect(const vec4 pos, const vec4 color,
//	vec<EntityVertex>& vertices, vec<u32>& indices, u32& ind, const u32 insert) {
//
//	vec<EntityVertex> myVerts = {
//		{ vec3 { pos.x, pos.y, 0 }, color, vec3(1), 0.f, vec3 {}, ivec4 {}, vec4 {} },
//		{ vec3 { pos.x, pos.w, 0 }, color, vec3(1), 0.f, vec3 {}, ivec4 {}, vec4 {} },
//		{ vec3 { pos.z, pos.w, 0 }, color, vec3(1), 0.f, vec3 {}, ivec4 {}, vec4 {} },
//		{ vec3 { pos.z, pos.y, 0 }, color, vec3(1), 0.f, vec3 {}, ivec4 {}, vec4 {} }
//	};
//
//	if (insert != -1) {
//		vertices[insert] = myVerts[0];
//		vertices[insert + 1] = myVerts[1];
//		vertices[insert + 2] = myVerts[2];
//		vertices[insert + 3] = myVerts[3];
//	}
//	else {
//		for (EntityVertex& vert : myVerts) vertices.emplace_back(vert);
//		for (u32 i : INDICES) indices.push_back(i + ind);
//		ind += 4;
//	}
//}
//
//const array<vec4, 16> GuiText::COLORS = {
//	Util::hexToColorVec("#000000"),
//	Util::hexToColorVec("#0000AA"),
//	Util::hexToColorVec("#00AA00"),
//	Util::hexToColorVec("#00AAAA"),
//	Util::hexToColorVec("#AA0000"),
//	Util::hexToColorVec("#b05cff"),
//	Util::hexToColorVec("#FFAA00"),
//	Util::hexToColorVec("#cccccc"),
//	Util::hexToColorVec("#555555"),
//	Util::hexToColorVec("#33a2f5"),
//	Util::hexToColorVec("#9fff80"),
//	Util::hexToColorVec("#7df4ff"),
//	Util::hexToColorVec("#ff739f"),
//	Util::hexToColorVec("#fd7dff"),
//	Util::hexToColorVec("#fffb82"),
//	Util::hexToColorVec("#ffffff")
//};
//
//const array<u32, 6> GuiText::INDICES = { 0, 1, 2, 2, 3, 0 };
//
//const vec4 GuiText::BG_MULTIPLE = { 0.3, 0.3, 0.35, 0.75 };