#pragma once

#include <functional>
#include <unordered_map>

#include "util/Types.h"

namespace {
	template<typename T> struct is_variant : std::false_type {};
	
	template<typename... Args>
	struct is_variant<std::variant<Args...>> : std::true_type {};
	
	template<typename T>
	inline constexpr bool is_variant_v = is_variant<T>::value;
	
	template<typename T> struct is_optional : std::false_type {};
	
	template<typename... Args>
	struct is_optional<std::optional<Args...>> : std::true_type {};
	
	template<typename T>
	inline constexpr bool is_optional_v = is_optional<T>::value;
}

template <typename R, typename C = nullptr_t>
class StringParser {
public:
	typedef R Data;
	typedef C Ctx;
	
	class Fn;

	using EXEC_ARGS = const vec<std::string_view>&;
	using EXEC_FN = std::function<R(C&, EXEC_ARGS)>;

	struct Fn {
		Fn(EXEC_FN exec): exec(exec) {};

		R operator()(C& ctx, EXEC_ARGS args) const {
			return exec(ctx, args);
		}

	private:
		EXEC_FN exec;
	};
	
	explicit StringParser() = default;
	
	template <typename... Args, typename Func>
	void addFn(const string& name, const Func& fn) {
		functions.emplace(name, [=, this](C& ctx, EXEC_ARGS strArgs) {
			std::tuple<Args...> args = {};
			parseStrArgs<std::tuple<Args...>>(strArgs, args, ctx);
			return std::apply(fn, args);
		});
	}
	
	template <typename... Args, typename Func>
	void addFnCtx(const string& name, const Func& fn) {
		functions.emplace(name, [=, this](C& ctx, EXEC_ARGS strArgs) {
			std::tuple<Args...> args = {};
			parseStrArgs<std::tuple<Args...>>(strArgs, args, ctx);
			return std::apply(fn, std::tuple_cat(std::tuple<C&>(ctx), args));
		});
	}
	
	template <typename... Args, typename Func>
	void addLiteralFn(const Func& fn) {
		addFn<Args...>("_", fn);
	}
	
	template <typename... Args, typename Func>
	void addLiteralFnCtx(const Func& fn) {
		addFnCtx<Args...>("_", fn);
	}
	
	const R parse(string str) const {
		const nullptr_t ctx {};
		str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
		return std::move(parseRaw(str, const_cast<C&>(ctx)));
	}
	
	const R parse(string str, C& ctx) const {
		str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
		return std::move(parseRaw(str, ctx));
	}

private:
	const R parseRaw(const std::string_view str, C& ctx = {}) const {
		if (strIsFunction(str)) {
			let func = parseFunction(str);
			const let f = functions.find(string(func.first));
			if (f == functions.end()) throw std::invalid_argument("Unknown function '" + string(func.first) + "'!");
			return f->second(ctx, func.second);
		}
		else {
			const let& f = functions.find("_");
			if (f == functions.end()) throw std::invalid_argument("No default function handler!");
			return f->second(ctx, { str });
		}
	}
	
	const usize findClosingParen(const std::string_view& str, usize start) const {
		usize levels = 0;
		for (usize i = start + 1; i < str.size(); i++) {
			if (str[i] == '(') levels++;
			else if (str[i] == ')') {
				if (levels > 0) levels--;
				else return i;
			}
		}
		
		throw std::invalid_argument("Mismatched parentheses.");
	}
	
	const bool strIsFunction(const std::string_view& str) const {
		return str.find_first_of('(') != string::npos;
	}
	
	const std::pair<std::string_view, vec<std::string_view>> parseFunction(const std::string_view& str) const {
		let nextParen = str.find_first_of('(');
		let nextComma = str.find_first_of(',');
		
		if (nextParen == string::npos || (nextComma != string::npos && nextComma < nextParen))
			throw std::invalid_argument("Not a function");
		
		let name = str.substr(0, nextParen);
		vec<std::string_view> args {};
		
		let end = findClosingParen(str, nextParen);
		let s = nextParen + 1;
		
		while (s <= end) {
			nextParen = str.find_first_of('(', s);
			nextComma = str.find_first_of(',', s);
			if (nextParen != string::npos && nextParen < nextComma) nextComma = findClosingParen(str, nextParen) + 1;
			if (nextComma == string::npos) nextComma = end;
			
			args.emplace_back(str.substr(s, nextComma - s));
			s = nextComma + 1;
		}
		
		return { name, args };
	}
	
	template <typename T, usize I, std::enable_if_t<std::is_same_v<T, string>, bool> = true>
	T parseStrArg(EXEC_ARGS args, C&) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return string(args[I]);
	}
	
	template <typename T, usize I, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	T parseStrArg(EXEC_ARGS args, C&) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return std::stoi(args[I].data());
	}
	
	template <typename T, usize I, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	T parseStrArg(EXEC_ARGS args, C&) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return std::stod(args[I].data());
	}
	
	template <typename T, usize I, usize VI = 0>
	T parseStrVariantArg(EXEC_ARGS args, C& ctx) {
		try {
			return parseStrArg<std::variant_alternative_t<VI, T>, I>(args, ctx);
		}
		catch (...) {}
		if constexpr (VI + 1 < std::variant_size_v<T>) {
			return parseStrVariantArg<T, I, VI + 1>(args, ctx);
		}
		else {
			throw std::invalid_argument("Argument does not match types required.");
		}
	}
	
	template<typename T, usize I, std::enable_if_t<is_variant_v<T>, bool> = true>
	T parseStrArg(EXEC_ARGS args, C& ctx) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return parseStrVariantArg<T, I>(args, ctx);
	}
	
	template<typename T, usize I, std::enable_if_t<is_optional_v<T>, bool> = true>
	T parseStrArg(EXEC_ARGS args, C& ctx) {
		if (I >= args.size() || args[I].empty()) return {};
		return parseStrArg<typename T::value_type, I>(args, ctx);
	}
	
	template <typename T, usize I, std::enable_if_t<std::is_same_v<T, R>, bool> = true>
	T parseStrArg(EXEC_ARGS args, C& ctx) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return parseRaw(parseStrArg<string, I>(args, ctx), ctx);
	}
	
	template <typename T, usize I = 0>
	void parseStrArgs(EXEC_ARGS strArgs, T& args, C& ctx) {
		std::get<I>(args) = parseStrArg<std::tuple_element_t<I, T>, I>(strArgs, ctx);
		if constexpr (I + 1 < std::tuple_size_v<T>) parseStrArgs<T, I + 1>(strArgs, args, ctx);
	}

	std::unordered_map<string, Fn> functions {};
};