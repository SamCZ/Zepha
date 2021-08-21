#include "Style.h"

const std::unordered_map<string, Gui::StyleRule> Gui::Style::RULE_STRINGS_TO_ENUMS = {
	{ "pos", StyleRule::POS },
	{ "size", StyleRule::SIZE },
	{ "margin", StyleRule::MARGIN },
	{ "padding", StyleRule::PADDING },
	{ "GAP", StyleRule::GAP },
	{ "layout", StyleRule::LAYOUT },
	{ "direction", StyleRule::DIRECTION },
	{ "h_align", StyleRule::H_ALIGN },
	{ "v_align", StyleRule::V_ALIGN },
	{ "cursor", StyleRule::CURSOR },
	{ "overflow", StyleRule::OVERFLOW },
	{ "background", StyleRule::BACKGROUND },
	{ "background_hover", StyleRule::BACKGROUND_HOVER },
	{ "content", StyleRule::CONTENT },
	{ "text_size", StyleRule::TEXT_SIZE },
	{ "text_color", StyleRule::TEXT_COLOR }
};