#pragma once

#include "Element.h"

#include "client/graph/Font.h"
//#include "game/atlas/asset/AtlasRef.h"

namespace Gui {
	
	/**
	 * Displays formatted text specified by the contact property.
	 */
	
	class TextElement: public Element {
	public:
		using Element::Element;
	
		virtual void updateElement() override;
		
	protected:
//		sptr<AtlasRef> tex;
//		optional<any> curBg;

	private:
		uptr<Font> font;
		usize hash = 0;

		void drawRect(const vec4 pos, const vec4 color,
			vec<EntityVertex>& vertices, vec<u32>& indices, u32& ind, const u32 insert = -1);

		static const vec4 BG_MULTIPLE;
		static const array<u32, 6> INDICES;
		static const array<vec4, 16> COLORS;
	};
}