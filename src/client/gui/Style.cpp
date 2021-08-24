#include "Style.h"

const std::unordered_map<string, Gui::Prop> Gui::Props::PROP_NAMES_TO_ENUM = {
	{ "id", Prop::ID },
	{ "class", Prop::CLASS },
	{ "pos", Prop::POS },
	{ "size", Prop::SIZE },
	{ "margin", Prop::MARGIN },
	{ "padding", Prop::PADDING },
	{ "gap", Prop::GAP },
	{ "layout", Prop::LAYOUT },
	{ "direction", Prop::DIRECTION },
	{ "h_align", Prop::H_ALIGN },
	{ "v_align", Prop::V_ALIGN },
	{ "cursor", Prop::CURSOR },
	{ "overflow", Prop::OVERFLOW },
	{ "background", Prop::BACKGROUND },
	{ "background_hover", Prop::BACKGROUND_HOVER },
	{ "content", Prop::CONTENT },
	{ "text_size", Prop::TEXT_SIZE },
	{ "text_color", Prop::TEXT_COLOR }
};