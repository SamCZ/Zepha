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
			
			BACKGROUND_COLOR,
			BACKGROUND_IMAGE
		};
		
		enum class Type {
			LITERAL,
			COLOR,
			LENGTH
		};
		
		Style() = default;
		Style(const std::unordered_map<Rule, std::any>& rules): rules(rules) {}
		
	private:
		
		/**
		 * Simple get. Returns an optional containing an any of
		 * the Rule's value, or an empty optional if the rule is not defined.
		 */
		
		const optional<any> get(Rule rule) const {
			const let it = rules.find(rule);
			if (it == rules.end()) return std::nullopt;
			return optional<any>(it->second);
		}
		
	public:
		
		/**
		 * Returns an optional of type T of the specified Rule's value,
		 * or an empty optional if the rule is not defined.
		 * Throws if the rule is defined to a different type.
		 */
		
		template<typename O, Type T = Type::LITERAL, std::enable_if<
			std::is_same_v<O, optional<typename O::value_type>> &&
		    T == Type::LITERAL, bool> = true>
		    	
        const O get(Rule rule) const {
			const let raw = get(rule);
			if (!raw) return raw;
			if (raw->type() == typeid(typename O::value_type)) return raw;
			throw std::runtime_error("Rule value is of an incorrect type.");
		}
		
		template<typename V, Type T = Type::LITERAL, std::enable_if_t<
		std::is_same_v<V, optional<typename V::value_type>>, bool> = true>
			const any& get(Rule rule) const {
			const any& v = get<typename V::value_type, T>(rule);
			if (&v == &ANY_MISSING) return V();
			if (v.type())
			return V(v)
			const let it = rules.find(rule);
			if (it == rules.end()) return ANY_MISSING;
			return it->second;
		}
		
		template<typename V, Type T = Type::LITERAL, std::enable_if_t<(!std::is_same_v<V, string> &&
		    !std::is_integral_v<V> && !std::is_floating_point_v<V>) && T == Type::LITERAL, bool> = true>
		    	
        const V get(Rule rule, std::optional<V> def = std::nullopt) const {
			const let it = rules.find(rule);
			if (it == rules.end() && def) return *def;
			if (it == rules.end()) throw std::runtime_error("Field is missing.");
			if (it->second.type() == typeid(V)) return std::any_cast<V>(it->second);
			throw std::runtime_error("Field is incorrect type.");
		}
		
		template<typename Str, Type T = Type::LITERAL, std::enable_if_t<
		    std::is_same_v<Str, string> && T == Type::LITERAL, bool> = true>
		    
		const Str get(Rule rule, std::optional<Str> def = std::nullopt) const {
			const let it = rules.find(rule);
			if (it == rules.end() && def) return *def;
			if (it == rules.end()) throw std::runtime_error("Field is missing.");
			if (it->second.type() == typeid(string)) return std::any_cast<string>(it->second);
			if (it->second.type() == typeid(const char*)) return std::any_cast<const char*>(it->second);
			throw std::runtime_error("Field is incorrect type.");
		}
		
		template<typename N, Type T = Type::LITERAL, std::enable_if_t<
			(std::is_integral_v<N> || std::is_floating_point_v<N>) && T == Type::LITERAL, bool> = true>
			
		const N get(Rule rule, std::optional<N> def = std::nullopt) const {
			const let it = rules.find(rule);
			if (it == rules.end() && def) return *def;
			if (it == rules.end()) throw std::runtime_error("Field is missing.");
			if (it->second.type() == typeid(N)) return std::any_cast<N>(it->second);
			if (it->second.type() == typeid(i8)) return static_cast<N>(std::any_cast<i8>(it->second));
			if (it->second.type() == typeid(i16)) return static_cast<N>(std::any_cast<i16>(it->second));
			if (it->second.type() == typeid(i32)) return static_cast<N>(std::any_cast<i32>(it->second));
			if (it->second.type() == typeid(i64)) return static_cast<N>(std::any_cast<i64>(it->second));
			if (it->second.type() == typeid(f32)) return static_cast<N>(std::any_cast<f32>(it->second));
			if (it->second.type() == typeid(f64)) return static_cast<N>(std::any_cast<f64>(it->second));
			if (it->second.type() == typeid(u8)) return static_cast<N>(std::any_cast<u8>(it->second));
			if (it->second.type() == typeid(u16)) return static_cast<N>(std::any_cast<u16>(it->second));
			if (it->second.type() == typeid(u32)) return static_cast<N>(std::any_cast<u32>(it->second));
			if (it->second.type() == typeid(u64)) return static_cast<N>(std::any_cast<u64>(it->second));
			if (it->second.type() == typeid(usize)) return static_cast<N>(std::any_cast<usize>(it->second));
			throw std::runtime_error("Field is incorrect type.");
		}
		
		template<typename V, Type C, std::enable_if_t<C == Type::COLOR, bool> = true>
		
		const V get(Rule rule, std::optional<V> def = std::nullopt) const {
			const std::any& v = get(rule);
			if (v.type() == typeid(void) && def) return *def;
			if (v.type() == typeid(void)) throw std::runtime_error("Field is missing with no default.");
			if (v.type() == typeid(vec4)) return std::any_cast<vec4>(v);
			if (v.type() == typeid(string)) return Util::hexToColorVec(std::any_cast<string>(v));
			if (v.type() == typeid(const char*)) return Util::hexToColorVec(string(std::any_cast<const char*>(v)));
			throw std::runtime_error("Field is incorrect type.");
		}
		
		template<typename I, Type L, std::enable_if_t<L == Type::LENGTH, bool> = true>
		
		I get(Rule rule, std::optional<I> def = std::nullopt) const {
			return get<i32>(rule, def);
		}
		
		std::unordered_map<Rule, std::any> rules {};
	
		const static any ANY_MISSING;
	};
}