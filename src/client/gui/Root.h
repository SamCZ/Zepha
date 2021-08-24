#pragma once

#include "client/gui/Gui.h"

#include "Element.h"
#include "client/Window.h"
#include "game/atlas/TextureAtlas.h"

/**
 * Handles rendering and processing events for a Gui tree.
 * Holds a `body` Box that is always the screen's size.
 */

namespace Gui {
	class Root {
	public:
		Root(Window& window, TextureAtlas& atlas);
		
		~Root();
		
		/**
		 * Creates an element from the props provided, optionally with children.
		 * Does not add it to the tree, use body->append() for that.
		 */
		
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		
		sptr<E> create(const Props& props = {}, const vec<sptr<Element>>& children = {}) {
			let elem = make_shared<E>(*this, stylesheets);
			elem->setProps(props);
			
			for (const let& child : children) elem->append(child);
			
			return elem;
		};
		
		/**
		 * Adds a stylesheet to the tree, which can provide
		 * styles for elements with specific classes.
		 */
		
		void addStylesheet(const StyleSheet& sheet);
		
		/** Processes mouse events. */
		void update();
		
		/** Renders the Gui tree. */
		void draw(Renderer& renderer);
		
		/** The list of stylesheets to apply to the document. */
		vec<StyleSheet> stylesheets;
		
		/** The body element, which is the root of the tree. */
		const sptr<Element> body;
		
		/** The clients texture atlas. */
		TextureAtlas& atlas;
		
	private:
		
		Window& window;
		vec<CallbackRef> callbacks {};
	};
}

