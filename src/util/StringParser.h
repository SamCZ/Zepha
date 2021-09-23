#pragma once

#include <iostream>
#include <functional>
#include <unordered_map>

#include "util/Types.h"

//template<typename R, typename... Args>
//struct Fn {
//	typedef std::function<R(Args...)> FN_TYPE;
//	Fn(const string& name, const FN_TYPE& fn): name(name), fn(fn) {}
//	const string name;
//	const FN_TYPE fn;
//};
//

class Fn {
	using EXEC_ARGS = const vec<std::string_view>&;
	using EXEC_FN = std::function<void(std::unordered_map<string, Fn>, EXEC_ARGS)>;
	
public:
	Fn(EXEC_FN exec): exec(exec) {};
	
	template <typename... Args, typename Func>
	static Fn create(const Func& fn) {
		using TYPES = std::tuple<Args...>;
		
		return Fn([=](std::unordered_map<string, Fn> functions, EXEC_ARGS strArgs) {
			TYPES args = {};
			parseStrArgs<TYPES>(strArgs, args);
			std::apply(fn, args);
		});
	}
	
	void operator()(std::unordered_map<string, Fn> functions, EXEC_ARGS args) const {
		return exec(functions, args);
	}
	
private:
	EXEC_FN exec;
	
	template <typename T, usize I, std::enable_if_t<std::is_same_v<T, string>, bool> = true>
	static T parseStrArg(EXEC_ARGS args) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return string(args[I]);
	}
	
	template <typename T, usize I, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	static T parseStrArg(EXEC_ARGS args) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return std::stoi(args[I].data());
	}
	
	template <typename T, usize I, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	static T parseStrArg(EXEC_ARGS args) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return std::stod(args[I].data());
	}
	
	template <typename T, usize I = 0>
	static void parseStrArgs(EXEC_ARGS strArgs, T& args) {
		std::get<I>(args) = parseStrArg<std::tuple_element_t<I, T>, I>(strArgs);
		if constexpr (I + 1 < std::tuple_size_v<T>) parseStrArgs<T, I + 1>(strArgs, args);
	}
};

template <typename R>
class StringParser {
public:
	using PARAM_TYPES = variant<string, int, float, R>;

	explicit StringParser(const std::unordered_map<string, Fn>& functions): functions(functions) {};

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

	const R parse(string str) const {
		str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
		let view = std::string_view(str);


		if (strIsFunction(view)) {
			let func = parseFunction(view);
			const let f = functions.find(string(func.first));
			if (f == functions.end()) throw std::invalid_argument("Unknown function '" + string(func.first) + "'!");
			f->second(functions, func.second);
		}
		else {
			const let& f = functions.find("_");
			if (f == functions.end()) throw std::invalid_argument("No default function handler!");
			f->second(functions, { view });
		}

		return R {};
	}

private:
//	template <class T, template <class...> class Template>
//	struct is_specialization : std::false_type {};
//
//	template <template <class...> class Template, class... Args>
//	struct is_specialization<Template<Args...>, Template> : std::true_type {};
//
//	template <typename T, usize I = 0>
//	T parseVariantVal(const std::string_view& str) {
//		try {
//			return parseVal<std::variant_alternative_t<I, T>>(str);
//		}
//		catch (...) {}
//		if constexpr (I + 1 < std::variant_size_v<T>) {
//			return parseVariantVal<T, I + 1>(str);
//		}
//		else {
//			throw std::invalid_argument("Argument does not match types required.");
//		}
//	}
//
//	template<typename T, std::enable_if_t<std::variant_size_v<T> != 0, bool> = true>
//	T parseVal(const std::string_view& str) {
//		return parseVariantVal<T>(str);
//	}

	std::unordered_map<string, Fn> functions {};

//	template<typename T, std::enable_if_t<is_specialization<T, optional>::value, bool> = true>
//	T parseVal(const std::string_view& str) {
//		try {
//
//		}
//	}

//	template<typename>
};