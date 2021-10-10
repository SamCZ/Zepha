#pragma once

#include "Element.h"

namespace Gui {
	
	/**
	 * A simple box element that may have background and/or children.
	 */
	
	class BoxElement: public Element {
	public:
		using Element::Element;
	
		virtual void updateElement() override;
		
	protected:
		optional<any> curBg;
		sptr<AtlasTexture> tex = nullptr;
	};
}