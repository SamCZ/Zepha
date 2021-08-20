#include "Element.h"

namespace Gui {
	class BoxElement: public Element {
	public:
		using Element::Element;
	
		virtual void updateElement() override;
		
	protected:
		optional<any> curBg;
	};
}