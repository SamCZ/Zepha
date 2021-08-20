#pragma once

#include <list>

#include "client/gui/Gui.h"
#include "client/gui/Style.h"
#include "world/dim/ent/DrawableEntity.h"

#include "util/Types.h"

class Window;
class Renderer;

namespace Gui {
	class Root;
	
	/**
	 * Base class for all Gui Elements.
	 * Represents an element within a Gui Root, which may be drawn to the screen.
	 */
	
	class Element {
		friend class BoxElement;
		
	public:
		struct Props {;
			string id {};
			vec<string> classes {};
			Style styles {};
		};
		
		Element(Root& root, vec<StyleSheet>& stylesheets): root(root), stylesheets(stylesheets) {}
		
		~Element();

		/** Sets the element's props to the struct specified. */
		virtual void setProps(const Props& props);

		/** Sets a style rule on the element. */
		virtual void setStyle(StyleRule style, const std::any& value);
		
		/** Recalculates the element based on its props. Call when props or stylesheets change. */
		virtual void updateElement();
		
		/** Draws the element to the screen. */
		virtual void draw(Renderer& renderer);
		
		/** Creates and prepends an element to this element. */
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		sptr<E> prepend(const Props& props = {}) {
			const let elem = make_shared<E>(root, stylesheets);
			elem->setProps(props);
			prepend(elem);
			return elem;
		};
		
		/** Prepends an existing element to this element. */
		sptr<Element> prepend(sptr<Element> elem) {
			children.push_front(elem);
			elem->parent = this;
			updateElement();
			return elem;
		}
		
		/** Creates and appends an element to this element. */
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		sptr<E> append(const Props& props = {}) {
			const let elem = make_shared<E>(root, stylesheets);
			elem->setProps(props);
			append(elem);
			return elem;
		};
		
		/** Appends an existing element to this element. */
		sptr<Element> append(sptr<Element> elem) {
			children.push_back(elem);
			elem->parent = this;
			updateElement();
			return elem;
		}
		
		/** Returns the element's computed size. */
		virtual ivec2 getComputedSize();
		
		/** Returns the element's computed size + margins. */
		virtual ivec2 getComputedOuterSize();
		
		/** Returns the element's computed content size, which is its size - padding. */
		virtual ivec2 getComputedContentSize();
		
		/** Returns the element's explicit size. Unspecified dimensions are -1. */
		virtual ivec2 getExplicitSize();
		
		/** Returns the element's computed position relative to its parent. */
		virtual ivec2 getComputedPos();
		
		/** Returns the element's computed position relative to the screen. */
		virtual ivec2 getComputedScreenPos();
		
		/** Gets a style value from the element's styles or the root's stylesheets. */
		const optional<any> getStyle(StyleRule rule) const {
			const optional<any> opt = props.styles.get(rule);
			if (opt) return *opt;
			for (const let& ss : stylesheets) {
				for (const string& className : props.classes) {
					const let& styles = ss.find(className);
					const optional<any> opt = styles->second.get(rule);
					if (opt) return *opt;
				}
			}
			return std::nullopt;
		}
		
		/** Gets a style value from the element's styles or the root's stylesheets. */
		template<typename V, ValueType T = ValueType::LITERAL>
		const optional<V> getStyle(StyleRule rule) const {
			const optional<V> opt = props.styles.get<V, T>(rule);
			if (opt) return *opt;
			for (const let& ss : stylesheets) {
				for (const string& className : props.classes) {
					const let& styles = ss.find(className);
					const optional<V> opt = styles->second.get<V, T>(rule);
					if (opt) return *opt;
				}
			}
			return std::nullopt;
		}
	
		/** Gets a style value from the element's styles or the root's stylesheets. */
		template<typename V, ValueType T = ValueType::LITERAL>
		const V getStyle(StyleRule rule, V def) const {
			const optional<V> opt = getStyle<V, T>(rule);
			if (opt) return *opt;
			return def;
		}

		/**
		 * Called by the root when the mouse position changes.
		 * Returns a boolean if the element or its children are hovered.
		 */
		 
		bool handleMouseHover(ivec2 mousePos, bool& pointer);
		
		/**
		 * Called by the root when the mouse clicks.
		 * Triggers a click interaction on the hovered element.
		 */

		bool handleMouseClick(u32 button, bool down);
		
	protected:
		Root& root;
		Props props;
		vec<StyleSheet>& stylesheets;

		DrawableEntity entity;
		Element* parent = nullptr;
		std::list<sptr<Element>> children;
		
		bool hovered = false;
		
		/** The screen offset of the parent. */
		ivec2 parentOffset {};
		
		/** The element's implicit size, as defined by the parent layout. */
		ivec2 layoutSize { -1, -1 };
		
		/** The element's implicit position, as defined by the parent layout. */
		ivec2 layoutPosition {};
		
		/** Updates child sizes and offsets based on layout styles. */
		virtual void layoutChildren();
	};
}