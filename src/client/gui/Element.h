#pragma once

#include <any>
#include <list>

#include "client/gui/Gui.h"
#include "client/gui/Style.h"
#include "world/dim/ent/DrawableEntity.h"

#include "util/Types.h"

class Window;
class Renderer;

namespace Gui {
	class Root;
	
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
		
		virtual void setProps(const Props& props);
		
		virtual void setStyle(Style::Rule style, const std::any& value);
		
		virtual void updateElement();
		
		virtual void draw(Renderer& renderer);
		
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		sptr<E> prepend(const Props& props = {}) {
			const let elem = make_shared<E>(root, stylesheets);
			elem->setProps(props);
			prepend(elem);
			return elem;
		};
		
		sptr<Element> prepend(sptr<Element> elem) {
			children.push_front(elem);
			elem->parent = this;
			updateElement();
			return elem;
		}
		
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		sptr<E> append(const Props& props = {}) {
			const let elem = make_shared<E>(root, stylesheets);
			elem->setProps(props);
			append(elem);
			return elem;
		};
		
		sptr<Element> append(sptr<Element> elem) {
			children.push_back(elem);
			elem->parent = this;
			updateElement();
			return elem;
		}
		
		virtual ivec2 getComputedSize();
		virtual ivec2 getComputedContentSize();
		virtual ivec2 getExplicitSize();
		virtual ivec2 getComputedPos();
		
		const optional<any> getStyle(Style::Rule rule) const {
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
		
		template<typename V, Style::Type T = Style::Type::LITERAL>
		const optional<V> getStyle(Style::Rule rule) const {
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
	
		template<typename V, Style::Type T = Style::Type::LITERAL>
		const V getStyle(Style::Rule rule, V def) const {
			const optional<V> opt = getStyle<V, T>(rule);
			if (opt) return *opt;
			return def;
		}

		void handleMouseMove(ivec2 mousePos);
		bool handleMouseClick(u32 button, bool down);
		
	protected:
		Root& root;
		Props props;
		vec<StyleSheet>& stylesheets;

		DrawableEntity entity;
		Element* parent = nullptr;
		std::list<sptr<Element>> children;
		
		bool hovered = false;
		
		ivec2 parentOffset {};
		ivec2 layoutSize { -1, -1 };
		ivec2 layoutPosition {};
		
		virtual void layoutChildren();
	};
}