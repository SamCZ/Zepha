#pragma once

#include <functional>

#include "client/gui/Gui.h"
#include "world/dim/ent/DrawableEntity.h"

#include "util/Types.h"
#include "client/gui/Style.h"

class Window;
class Renderer;

namespace Gui {
	class Root;
	
	/**
	 * Base class for all Gui Elements.
	 * Represents an element within a Gui Root, which may be drawn to the screen.
	 */
	
	class Element {
		friend class Root;
		friend class BoxElement;
		friend class TextElement;
		
	public:
		Element(Root& root, vec<StyleSheet>& stylesheets): root(root), stylesheets(stylesheets) {}
		
		~Element();
		
		/** Gets a reference to the element's props. */
		const Props& getProps() const;

		/** Sets the element's props to the struct specified. */
		virtual void setProps(const Props& props);

		/** Sets a style rule on the element. */
		virtual void setProp(Prop prop, const std::any& value);
		
		/** Recalculates the element based on its props. Call when props or stylesheets change. */
		virtual void updateElement();
		
		/** Draws the element to the screen. */
		virtual void draw(Renderer& renderer);
		
		sptr<Element> get(u32 ind);
		
		sptr<Element> get(const string& id);
		
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		sptr<E> get(u32 ind) {
			return std::dynamic_pointer_cast<E>(get(ind));
		}
		
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
			children.insert(children.begin(), elem);
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
		
		void clear();
		
		void remove();
		
		void onClick(const std::function<void(i32, bool)>& cb);
		
		/** Returns the element's computed size. */
		virtual vec2 getComputedSize() const;
		
		/** Returns the element's computed content size, which is its size - padding. */
		virtual vec2 getComputedContentSize() const;
		
		/** Returns the element's explicit size. Unspecified dimensions are -1. */
		virtual vec2 getExplicitSize() const;
		
		/** Returns the element's computed position relative to its parent. */
		virtual vec2 getComputedPos() const;
		
		/** Returns the element's computed position relative to the screen. */
		virtual vec2 getComputedScreenPos() const;
		
		/** Returns the element's explicit position. Unspecified dimensions are nan. */
		virtual vec2 getExplicitPos() const;
		
		/** Gets a style value from the element's styles or the root's stylesheets. */
		const optional<any> getStyle(Prop rule) const {
			const optional<any> opt = props.get(rule);
			if (opt) return *opt;
			const let& classes = props.get<vec<string>>(Prop::CLASS);
			if (!classes) return std::nullopt;
			for (const let& ss : stylesheets) {
				for (const string& className : *classes) {
					const let& styles = ss.find(className);
					if (styles == ss.end()) continue;
					const optional<any> opt = styles->second.get(rule);
					if (opt) return *opt;
				}
			}
			return std::nullopt;
		}
		
		/** Gets a generic value from the element's styles or the root's stylesheets. */
		template<typename V, Type T = Type::LITERAL, std::enable_if_t<T != Type::LENGTH, bool> = true>
			
		const optional<V> getStyle(Prop rule) const {
			const optional<V> opt = props.get<V, T>(rule);
			if (opt) return *opt;
			const let& classes = props.get<vec<string>>(Prop::CLASS);
			if (!classes) return std::nullopt;
			for (const let& ss : stylesheets) {
				for (const string& className : *classes) {
					const let& styles = ss.find(className);
					if (styles == ss.end()) continue;
					const optional<V> opt = styles->second.get<V, T>(rule);
					if (opt) return *opt;
				}
			}
			return std::nullopt;
		}
		
		/** Gets a LENGTH value from the element's styles or the root's stylesheets. */
		template<typename V, Type T = Type::LITERAL, std::enable_if_t<T == Type::LENGTH, bool> = true>
		
		const optional<V> getStyle(Prop rule) const {
			ExpressionInfo info = getExpr();
			const optional<V> opt = props.get<V, T>(rule, info);
			if (opt) return *opt;
			const let& classes = props.get<vec<string>>(Prop::CLASS);
			if (!classes) return std::nullopt;
			for (const let& ss : stylesheets) {
				for (const string& className : *classes) {
					const let& styles = ss.find(className);
					if (styles == ss.end()) continue;
					const optional<V> opt = styles->second.get<V, T>(rule, info);
					if (opt) return *opt;
				}
			}
			return std::nullopt;
		}
		
		/** Gets a LENGTH value from the element's styles or the root's stylesheets. */
		template<typename V, Type T = Type::LITERAL, std::enable_if_t<T == Type::LENGTH, bool> = true>
		
		const optional<V> getStyleWithExpr(Prop rule, const ExpressionInfo& expr) const {
        	const optional<V> opt = props.get<V, T>(rule, expr);
			if (opt) return *opt;
			const let& classes = props.get<vec<string>>(Prop::CLASS);
			if (!classes) return std::nullopt;
			for (const let& ss : stylesheets) {
				for (const string& className : *classes) {
					const let& styles = ss.find(className);
					if (styles == ss.end()) continue;
					const optional<V> opt = styles->second.get<V, T>(rule, expr);
					if (opt) return *opt;
				}
			}
			return std::nullopt;
		}
	
		/** Gets a style value from the element's styles or the root's stylesheets. */
		template<typename V, Type T = Type::LITERAL>
		
		const V getStyle(Prop rule, V def) const {
			const optional<V> opt = getStyle<V, T>(rule);
			if (opt) return *opt;
			return def;
		}
	
		/** Gets a LENGTH value from the element's styles or the root's stylesheets, with a custom ExpressionInfo. */
		template<typename V, Type T = Type::LITERAL, std::enable_if_t<T == Type::LENGTH, bool> = true>
			
		const V getStyleWithExpr(Prop rule, V def, const ExpressionInfo& info) const {
			const optional<V> opt = getStyleWithExpr<V, T>(rule, info);
			if (opt) return *opt;
			return def;
		}
		
		/** Returns an ExpressionInfo object for evaluating Lengths. */
		virtual ExpressionInfo getExpr() const;
		
	protected:

		/**
		 * Called by the root when the mouse position changes.
		 * Returns a boolean if the element or its children are hovered.
		 */
		 
		bool handleMouseHover(ivec2 mousePos, bool& pointer);
		
		/**
		 * Called by the root when the mouse clicks.
		 * Triggers a click interaction on the hovered element.
		 */

		bool handleMouseClick(ivec2 mousePos, u32 button, bool down);
		
		Root& root;
		Props props;
		vec<StyleSheet>& stylesheets;

		DrawableEntity entity;
		Element* parent = nullptr;
		vec<sptr<Element>> children;
		
		bool hovered = false;
		std::function<void(u32, bool)> clickCb = nullptr;
		
		/** The screen offset of the parent. */
		vec2 parentOffset {};
		
		/** The last computed size of the element. */
		mutable vec2 computedSize {};
		
		/** The element's implicit size, as defined by the parent layout. */
		vec2 layoutSize { -1, -1 };
		
		/** The element's implicit position, as defined by the parent layout. */
		vec2 layoutPosition {};
		
		/** Updates child sizes and offsets based on layout styles. */
		virtual void layoutChildren();
	};
}