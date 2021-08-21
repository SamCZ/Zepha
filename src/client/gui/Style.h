#pragma once

#include "client/gui/Gui.h"

#include "client/gui/Expression.h"

namespace Gui {
	enum class StyleRule {
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
	
	enum class ValueType {
		LITERAL,
		COLOR,
		LENGTH
	};
	
	class Style {
	public:
		Style() = default;
		Style(const std::unordered_map<StyleRule, any>& rules): rules(rules) {}
		
		/**
		 * Simple get. Returns an optional containing an any of
		 * the Rule's value, or an empty optional if the rule is not defined.
		 */
		
		const optional<any> get(StyleRule rule) const {
			const let it = rules.find(rule);
			if (it == rules.end()) return std::nullopt;
			return optional<any>(it->second);
		}
	
		/**
		 * Returns an optional of type O of the specified Rule's value,
		 * or an empty optional if the rule is not defined, or not the type V.
		 */
		
		template<typename V, ValueType T = ValueType::LITERAL, std::enable_if_t<
			!(std::is_integral_v<V> ||
			std::is_floating_point_v<V> ||
		    std::is_same_v<V, string>) &&
			T == ValueType::LITERAL, bool> = true>
		    	
        const optional<V> get(StyleRule rule) const {
			const optional<any> raw = get(rule);
			if (!raw || raw->type() != typeid(V)) return std::nullopt;
			return any_cast<V>(*raw);
		}
		
		/**
		 * get<O, T> specialization for string-like values.
		 */
		
		template<typename S, ValueType T = ValueType::LITERAL, std::enable_if_t<
			std::is_same_v<S, string> &&
			T == ValueType::LITERAL, bool> = true>
		    
		const optional<S> get(StyleRule rule) const {
			const optional<any> raw = get(rule);
			if (!raw) return std::nullopt;
			if (raw->type() == typeid(string)) return any_cast<string>(*raw);
//			if (raw->type() == typeid(const char*)) return any_cast<const char*>(*raw);
			return std::nullopt;
		}
		
		/**
		 * get<O, T> specialization for numeric values.
		 */
		 
		template<typename N, ValueType T = ValueType::LITERAL, std::enable_if_t<
			(std::is_integral_v<N> ||
		    std::is_floating_point_v<N>) &&
			T == ValueType::LITERAL, bool> = true>
			
        const optional<N> get(StyleRule rule) const {
        	const optional<any> raw = get(rule);
        	if (!raw) return std::nullopt;
        	if (raw->type() == typeid(N)) return any_cast<N>(*raw);
//        	if (raw->type() == typeid(i8)) return static_cast<N>(any_cast<i8>(*raw));
//        	if (raw->type() == typeid(i16)) return static_cast<N>(any_cast<i16>(*raw));
//        	if (raw->type() == typeid(i32)) return static_cast<N>(any_cast<i32>(*raw));
//        	if (raw->type() == typeid(i64)) return static_cast<N>(any_cast<i64>(*raw));
//        	if (raw->type() == typeid(f32)) return static_cast<N>(any_cast<f32>(*raw));
//        	if (raw->type() == typeid(f64)) return static_cast<N>(any_cast<f64>(*raw));
//        	if (raw->type() == typeid(u8)) return static_cast<N>(any_cast<u8>(*raw));
//        	if (raw->type() == typeid(u16)) return static_cast<N>(any_cast<u16>(*raw));
//        	if (raw->type() == typeid(u32)) return static_cast<N>(any_cast<u32>(*raw));
//        	if (raw->type() == typeid(u64)) return static_cast<N>(any_cast<u64>(*raw));
//        	if (raw->type() == typeid(usize)) return static_cast<N>(any_cast<usize>(*raw));
			return std::nullopt;
		}
		
		/**
		 * Returns an optional of the specified Rule's value,
		 * which is interpreted as a vec4 color from several different formats.
		 */
		
		template<typename V, ValueType C, std::enable_if_t<
			std::is_same_v<V, vec4> &&
			C == ValueType::COLOR, bool> = true>
		
		const optional<V> get(StyleRule rule, optional<V> def = std::nullopt) const {
			const optional<any> raw = get(rule);
			if (!raw) return std::nullopt;
			if (raw->type() == typeid(void) && def) return *def;
			if (raw->type() == typeid(void)) throw std::runtime_error("Field is missing with no default.");
			if (raw->type() == typeid(vec4)) return any_cast<vec4>(*raw);
			try {
				if (raw->type() == typeid(string)) return Util::hexToColorVec(any_cast<string>(*raw));
//				if (raw->type() == typeid(const char*)) return Util::hexToColorVec(string(any_cast<const char*>(*raw)));
			}
			catch (std::exception) {
				return std::nullopt;
			}
			return std::nullopt;
		}
		
		/**
		 * Returns an optional of the specified Rule's value,
		 * which is interpreted as a length.
		 */
		
		template<typename N, ValueType L, std::enable_if_t<
			(std::is_integral_v<N> || std::is_floating_point_v<N>) &&
			L == ValueType::LENGTH, bool> = true>
		
		optional<N> get(StyleRule rule, const ExpressionInfo& info) const {
			let raw = get<Gui::Expression>(rule);
			if (!raw) return std::nullopt;
			return raw->eval(info);
		}
		
		/**
		 * Returns an optional of the specified Rule's value,
		 * which is interpreted as a length.
		 */
		
		template<typename VN, ValueType L, std::enable_if_t<
			(std::is_integral_v<typename VN::value_type> || std::is_floating_point_v<typename VN::value_type>) &&
			std::is_same_v<VN, glm::vec<VN::length(), typename VN::value_type>> &&
			L == ValueType::LENGTH, bool> = true>
			
		optional<VN> get(StyleRule rule, const ExpressionInfo& info) const {
			let raw = get<array<Gui::Expression, VN::length()>>(rule);
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
		 
		template<typename V, ValueType T = ValueType::LITERAL>
		
		const V get(StyleRule rule, V def) const {
			const optional<V> raw = get<V, T>(rule);
			if (!raw) return def;
			return *raw;
		}
		
		std::unordered_map<StyleRule, any> rules {};
	
		const static std::unordered_map<string, StyleRule> RULE_STRINGS_TO_ENUMS;
	};
	
	typedef std::unordered_map<string, Style> StyleSheet;
}