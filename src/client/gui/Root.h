#include <unordered_map>

#include "client/gui/Gui.h"

#include "Element.h"
#include "util/Types.h"
#include "client/Window.h"
#include "game/atlas/TextureAtlas.h"

namespace Gui {
	class Root {
	public:
		Root(Window& window, TextureAtlas& atlas);
		
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		sptr<E> create(const Element::Props& props = {}, const vec<sptr<Element>>& children = {}) {
			let elem = make_shared<E>(*this);
			elem->setProps(props);
			
			for (const let& child : children) elem->append(child);
			
			return elem;
		};
		
		void addStylesheet(const std::unordered_map<string, Style>& sheet);
		
		const vec<std::unordered_map<string, Style>>& getStylesheets();
		
		void draw(Renderer& renderer);
		
		const sptr<Element> body;
		
		TextureAtlas& atlas;
		
	private:
		Window& window;
		Window::RCBLock lock;
		
		vec<std::unordered_map<string, Style>> stylesheets;
	};
}

