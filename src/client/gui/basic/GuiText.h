//#pragma once
//
//#include "client/gui/Element.h"
//
//#include "client/graph/Font.h"
//
//class TextureAtlas;
//class LuaGuiElement;
//
///**
// * Renders text, with formatting escape codes.
// *
// * `` - Raw grave character
// * `  - Single pixel space
// * `c[0-f] - Preset colors
// * `cr - Reset to the global text color
// * `#[hex code] - Color with an arbitrary hex code
// * `b - Bold
// * `i - Italic
// * `u - Underline
// * `s - Strikethrough
// * `r - Reset to default formatting
// */
//
//class GuiText : public Element {
//public:
//	using Element::Element;
//
//	/** Creates a text component from a serialized lua element. */
//	static sptr<GuiText> fromSerialized(const LuaGuiElement& elem, Window& window, TextureAtlas& textures, ivec2 bounds);
//
//	/** Creates a new text component. */
//	void create(vec2 scale, vec4 padding, vec4 textColor, vec4 backgroundColor, Font font);
//
//	/** Returns the width in display pixels of the text. */
//	u32 getWidth();
//
//	/** Sets the text to the string provided. */
//	void setText(string text);
//
//	/** Returns the current text string. */
//	string getText();
//
//private:
//	Font font;
//
//	void drawRect(const vec4 pos, const vec4 color,
//		vec<EntityVertex>& vertices, vec<u32>& indices, u32& ind, const u32 insert = -1);
//
//	string text;
//	vec4 textColor {};
//	vec4 backgroundColor {};
//
//	u32 width = 0;
//
//	static const array<vec4, 16> COLORS;
//	static const array<u32, 6> INDICES;
//	static const vec4 BG_MULTIPLE;
//};
//
