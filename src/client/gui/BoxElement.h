#include "Element.h"

namespace Gui {
	class BoxElement: public Element {
	public:
		using Element::Element;
	
	protected:
		virtual void updateElement() override;

		virtual void layoutChildren() override;
	};
}