#include "Element.h"

#include "game/atlas/asset/AtlasRef.h"

namespace Gui {
	class BoxElement: public Element {
	public:
		using Element::Element;
	
		virtual void updateElement() override;
		
	protected:
		sptr<AtlasRef> tex;
		optional<any> curBg;
	};
}