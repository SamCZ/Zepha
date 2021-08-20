#pragma once

#include "client/gui/Gui.h"

namespace Gui {
	class Style {
	public:
		enum class Rule {
			PADDING,
			WIDTH,
			HEIGHT,
			TOP,
			LEFT,
			
			GAP,
			LAYOUT,
			DIRECTION,
			H_ALIGN,
			V_ALIGN,
			
			OVERFLOW,
			
			BACKGROUND
		};
		
		enum class Type {
			LITERAL,
			COLOR,
			LENGTH
		};
		
		Style() = default;
		Style(const std::unordered_map<Rule, any>& rules): rules(rules) {}
		
		/**
		 * Simple get. Returns an optional containing an any of
		 * the Rule's value, or an empty optional if the rule is not defined.
		 */
		
		const optional<any> get(Rule rule) const {
			const let it = rules.find(rule);
			if (it == rules.end()) return std::nullopt;
			return optional<any>(it->second);
		}
	
		/**
		 * Returns an optional of type O of the specified Rule's value,
		 * or an empty optional if the rule is not defined.
		 * Throws if the rule is defined to a different type.
		 */
		
		template<typename V, Type T = Type::LITERAL, std::enable_if_t<
			!(std::is_integral_v<V> ||
			std::is_floating_point_v<V> ||
		    std::is_same_v<V, string>) &&
		    T == Type::LITERAL, bool> = true>
		    	
        const optional<V> get(Rule rule) const {
			const optional<any> raw = get(rule);
			if (!raw) return std::nullopt;
			if (raw->type() == typeid(V)) return any_cast<V>(*raw);
			throw std::runtime_error("Rule value is of an incorrect type.");
		}
		
		/**
		 * get<O, T> specialization for string-like values.
		 */
		
		template<typename S, Type T = Type::LITERAL, std::enable_if_t<
			std::is_same_v<S, string> &&
	        T == Type::LITERAL, bool> = true>
		    
		const optional<S> get(Rule rule) const {
			const optional<any> raw = get(rule);
			if (!raw) return std::nullopt;
			if (raw->type() == typeid(string)) return any_cast<string>(*raw);
//			if (raw->type() == typeid(const char*)) return any_cast<const char*>(*raw);
			throw std::runtime_error("Rule value is of an incorrect type.");
		}
		
		/**
		 * get<O, T> specialization for numeric values.
		 */
		 
		template<typename N, Type T = Type::LITERAL, std::enable_if_t<
			(std::is_integral_v<N> ||
		    std::is_floating_point_v<N>) &&
		    T == Type::LITERAL, bool> = true>
			
        const optional<N> get(Rule rule) const {
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
			throw std::runtime_error("Rule value is of an incorrect type.");
		}
		
		/**
		 * Returns an optional of the specified Rule's value,
		 * which is interpreted as a vec4 color from several different formats.
		 * Throws if the rule is defined to a different type.
		 */
		
		template<typename V, Type C, std::enable_if_t<
			std::is_same_v<V, vec4> &&
			C == Type::COLOR, bool> = true>
		
		const optional<V> get(Rule rule, optional<V> def = std::nullopt) const {
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
			throw std::runtime_error("Rule value is of an incorrect type.");
		}
		
		/**
		 * Returns an optional of the specified Rule's value,
		 * which is interpreted as a length.
		 * Throws if the rule is defined to a different type.
		 */
		
		template<typename N, Type L, std::enable_if_t<
			(std::is_integral_v<N> || std::is_floating_point_v<N>) &&
			L == Type::LENGTH, bool> = true>
		
		optional<N> get(Rule rule) const {
			return get<N>(rule);
		}
		
		/**
		 * Returns the specified Rule's value as a V,
		 * or the default value provided as the second parameter.
		 * Throws if the rule is defined to a different type.
		 */
		 
		template<typename V, Type T = Type::LITERAL>
		
		const V get(Rule rule, V def) const {
			const optional<V> raw = get<V, T>(rule);
			if (!raw) return def;
			return *raw;
		}
		
		std::unordered_map<Rule, any> rules {};
	
		const static any ANY_MISSING;
	};
	
	typedef std::unordered_map<string, Style> StyleSheet;
}