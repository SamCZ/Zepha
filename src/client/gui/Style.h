#pragma once

#include "client/gui/Gui.h"

#include "client/gui/Expression.h"

namespace Gui {
	enum class Prop {
		ID,
		CLASS,
		VISIBLE,
		
		POS,
		SIZE,
		MARGIN,
		PADDING,
		
		GAP,
		LAYOUT,
		DIRECTION,
		H_ALIGN,
		V_ALIGN,
		
		CURSOR,
		OVERFLOW,
		BACKGROUND,
		BACKGROUND_HOVER,
		
		CONTENT,
		TEXT_SIZE,
		TEXT_COLOR
	};
	
	enum class Type {
		LITERAL,
		COLOR,
		LENGTH
	};
	
	class Props {
	public:
		Props() = default;
		Props(const std::unordered_map<Prop, any>& props): props(props) {}
		
		/**
		 * Simple get. Returns an optional containing an any of
		 * the Rule's value, or an empty optional if the rule is not defined.
		 */
		
		const optional<any> get(Prop prop) const {
			const let it = props.find(prop);
			if (it == props.end()) return std::nullopt;
			return optional<any>(it->second);
		}
	
		/**
		 * Returns an optional of type O of the specified Rule's value,
		 * or an empty optional if the rule is not defined, or not the type V.
		 */
		
		template<typename V, Type T = Type::LITERAL, std::enable_if_t<
			!(std::is_integral_v<V> ||
			std::is_floating_point_v<V> ||
		    std::is_same_v<V, string>) &&
		    T == Type::LITERAL, bool> = true>
		    	
        const optional<V> get(Prop prop) const {
            const optional<any> raw = get(prop);
			if (!raw || raw->type() != typeid(V)) return std::nullopt;
			return any_cast<V>(*raw);
		}
		
		/**
		 * get<O, T> specialization for string-like values.
		 */
		
		template<typename S, Type T = Type::LITERAL, std::enable_if_t<
			std::is_same_v<S, string> &&
			T == Type::LITERAL, bool> = true>
		    
		const optional<S> get(Prop prop) const {
			const optional<any> raw = get(prop);
			if (!raw) return std::nullopt;
			if (raw->type() == typeid(string)) return any_cast<string>(*raw);
			return std::nullopt;
		}
		
		/**
		 * get<O, T> specialization for numeric values.
		 */
		 
		template<typename N, Type T = Type::LITERAL, std::enable_if_t<
			(std::is_integral_v<N> ||
		    std::is_floating_point_v<N>) &&
		    T == Type::LITERAL, bool> = true>
			
        const optional<N> get(Prop prop) const {
            const optional<any> raw = get(prop);
        	if (!raw) return std::nullopt;
        	if (raw->type() == typeid(N)) return any_cast<N>(*raw);
			return std::nullopt;
		}
		
		/**
		 * Returns an optional of the specified Rule's value,
		 * which is interpreted as a vec4 color from several different formats.
		 */
		
		template<typename V, Type C, std::enable_if_t<
			std::is_same_v<V, vec4> &&
			C == Type::COLOR, bool> = true>
		
		const optional<V> get(Prop prop, optional<V> def = std::nullopt) const {
			const optional<any> raw = get(prop);
			if (!raw) return std::nullopt;
			if (raw->type() == typeid(void) && def) return *def;
			if (raw->type() == typeid(void)) throw std::runtime_error("Field is missing with no default.");
			if (raw->type() == typeid(vec4)) return any_cast<vec4>(*raw);
			try { if (raw->type() == typeid(string)) return Util::hexToColorVec(any_cast<string>(*raw)); }
			catch (std::exception) { return std::nullopt; }
			return std::nullopt;
		}
		
		/**
		 * Returns an optional of the specified Rule's value,
		 * which is interpreted as a length.
		 */
		
		template<typename N, Type L, std::enable_if_t<
			(std::is_integral_v<N> || std::is_floating_point_v<N>) &&
			L == Type::LENGTH, bool> = true>
		
		optional<N> get(Prop prop, const ExpressionInfo& info) const {
			let raw = get<Gui::Expression>(prop);
			if (!raw) return std::nullopt;
			return raw->eval(info);
		}
		
		/**
		 * Returns an optional of the specified Rule's value,
		 * which is interpreted as a length.
		 */
		
		template<typename VN, Type L, std::enable_if_t<
			(std::is_integral_v<typename VN::value_type> || std::is_floating_point_v<typename VN::value_type>) &&
			std::is_same_v<VN, glm::vec<VN::length(), typename VN::value_type>> &&
			L == Type::LENGTH, bool> = true>
			
		optional<VN> get(Prop prop, const ExpressionInfo& info) const {
			let raw = get<array<Gui::Expression, VN::length()>>(prop);
			if (!raw) return std::nullopt;
			VN vec;
			for (usize i = 0; i < VN::length(); i++) vec[i] = (*raw)[i].eval(info);
			return vec;
		}
		
		/**
		 * Returns the specified Rule's value as a V,
		 * or the default value provided as the second parameter.
		 * Throws if the rule is defined to a different type.
		 */
		 
		template<typename V, Type T = Type::LITERAL>
		
		const V get(Prop prop, V def) const {
			const optional<V> raw = get<V, T>(prop);
			if (!raw) return def;
			return *raw;
		}
		
		std::unordered_map<Prop, any> props {};
	
		const static std::unordered_map<string, Prop> PROP_NAMES_TO_ENUM;
	};
	
	typedef std::unordered_map<string, Props> StyleSheet;
}