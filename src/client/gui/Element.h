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
		
		Element(Root& root): root(root) {}
		
		~Element();
		
		void setProps(const Props& props);
		
		const std::any& getStyle(Style::Rule style) const;
		void setStyle(Style::Rule style, const std::any& value);
		
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		sptr<E> prepend(const Props& props = {}) {
			const let elem = make_shared<E>(root);
			elem->setProps(props);
			prepend(elem);
			return elem;
		};
		
		sptr<Element> prepend(sptr<Element> elem) {
			children.push_front(elem);
			elem->parent = this;
			return elem;
		}
		
		template<typename E, std::enable_if_t<std::is_base_of_v<Element, E>, bool> = true>
		sptr<E> append(const Props& props = {}) {
			const let elem = make_shared<E>(root);
			elem->setProps(props);
			append(elem);
			return elem;
		};
		
		sptr<Element> append(sptr<Element> elem) {
			children.push_back(elem);
			elem->parent = this;
			return elem;
		}
		
		virtual ivec2 getComputedSize();
		virtual ivec2 getComputedContentSize();
		virtual ivec2 getExplicitSize();
		virtual ivec2 getComputedPos();
		
		template<typename V, Style::Type T = Style::Type::LITERAL>
		const V getStyle(Style::Rule rule, std::optional<V> def = std::nullopt) const {
			return props.styles.get<V, T>(rule, def);
		}

		void handleMouseMove(ivec2 mousePos);
		bool handleMouseClick(u32 button, bool down);

		virtual void draw(Renderer& renderer);
		
	protected:
		Root& root;
		Props props;
		DrawableEntity entity;
		
		Element* parent = nullptr;
		std::list<sptr<Element>> children;
		
		ivec2 layoutSize { -1, -1 };
		ivec2 layoutPosition {};
		
		virtual void updateElement() {};
		
		virtual void layoutChildren() {};
	};
}