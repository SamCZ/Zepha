#pragma once

#include <functional>
#include <unordered_map>

#include "util/Types.h"

/** SFINAE Helpers. */
namespace {
	/** Struct used for identifying variants in SFINAE. */
	template<typename T> struct is_variant : std::false_type {};
	
	template<typename... Args>
	struct is_variant<std::variant<Args...>> : std::true_type {};
	
	/** Const bool used for identifying variants in SFINAE. */
	template<typename T>
	inline constexpr bool is_variant_v = is_variant<T>::value;
	
	/** Struct used for identifying optionals in SFINAE. */
	template<typename T> struct is_optional : std::false_type {};
	
	template<typename... Args>
	struct is_optional<std::optional<Args...>> : std::true_type {};
	
	/** Const bool used for identifying optionals in SFINAE. */
	template<typename T>
	inline constexpr bool is_optional_v = is_optional<T>::value;
}

/**
 * Exposes a method to parse a string containing recursive functions,
 * e.g: "tint(0, crop(0, 0, 16, 16, my_texture))".
 * Functions are defined using template parameters and coerced from the string,
 * executed in the order that they are used in the string and used to create an output.
 *
 * @tparam R - The type that parse() and all declared functions will return.
 * @tparam C - An optional context type, which an instance of can be passed to parse() and accessed in functions.
 */

template <typename R, typename C = std::nullptr_t>
class StringParser {
	
	/** A type alias for a vector of string parameters that will be passed into a Fn. */
	using STR_ARGS = const vec<std::string_view>&;
	
	/** A wrapped function that will accept an STR_ARGS and a Context, and execute a defined function with them. */
	using PARSE_FN = std::function<R(C&, STR_ARGS)>;
	
public:
	
	/** The data type of this parser. */
	typedef R Data;
	
	/** The context type of this parser. Will be nullptr_t if no context type is specified. */
	typedef C Ctx;
	
	explicit StringParser() = default;
	
	/** If set to true, when an unknown function is read it will be passed to the literal function as a string. */
	void setUnknownFnsAreLiteral(bool state) {
		unknownFnsAreLiteral = state;
	}
	
	/**
	 * Adds a function to the functions map.
	 * For a function to be valid, it must only have parameters that are
	 * integral, floating point, strings, the Data type, or optionals & variants of them.
	 * It must also return a Data type. The Args type parameter must match the function's types.
	 * If the function needs access to the Context, use addFnCtx() instead.
	 *
	 * @tparam Args - The argument types of the function.
	 * @param name - The name of the function.
	 * @param fn - The function lambda.
	 */
	 
	template <typename... Args, typename Func>
	void addFn(const string& name, const Func& fn) {
		functions.emplace(name, [=, this](C& ctx, STR_ARGS strArgs) {
			std::tuple<Args...> args = {};
			parseStrArgs<std::tuple<Args...>>(strArgs, args, ctx);
			return std::apply(fn, args);
		});
	}
	
	/**
	 * Adds a function to the functions map that has access to the Ctx.
	 * The same restrictions for a function apply, but the function
	 * must also accept a reference to a Ctx before all other arguments.
	 * This parameter should not be specified in Args.
	 *
	 * @tparam Args - The argument types of the function.
	 * @param name - The name of the function.
	 * @param fn - The function lambda.
	 */
	 
	template <typename... Args, typename Func>
	void addFnCtx(const string& name, const Func& fn) {
		functions.emplace(name, [=, this](C& ctx, STR_ARGS strArgs) {
			std::tuple<Args...> args = {};
			parseStrArgs<std::tuple<Args...>>(strArgs, args, ctx);
			return std::apply(fn, std::tuple_cat(std::tuple<C&>(ctx), args));
		});
	}
	
	/**
	 * Shortcut to add the literal function, which is called when a string literal is found.
	 * This is shorthand for calling addFn<string>("_", ...).
	 * The literal function must only accept a single string parameter.
	 *
	 * @param fn - The literal function lambda.
	 */
	
	template <typename Func>
	inline void addLiteralFn(const Func& fn) {
		addFn<string>("_", fn);
	}
	
	/**
	 * Shortcut to add the literal function that has access to the Ctx.
	 * The same restrictions for a literal function apply, but the function
	 * must also accept a reference to a Ctx before the string parameter.
	 *
	 * @param fn - The literal function lambda.
	 */
	 
	template <typename Func>
	inline void addLiteralFnCtx(const Func& fn) {
		addFnCtx<string>("_", fn);
	}
	
	/**
	 * Parses a string using the functions defined.
	 * This method may only be called if no context was specified.
	 *
	 * @param str - The string to parse.
	 * @returns the parsed result.
	 */
	
	const R parse(string str) const {
		const std::nullptr_t ctx {};
		str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
		return std::move(parseRaw(str, const_cast<C&>(ctx)));
	}
	
	/**
	 * Parses a string using the functions defined.
	 *
	 * @param str - The string to parse.
	 * @param ctx - The context to parse with, passed to all functions that use Ctx.
	 * @returns the parsed result.
	 */
	 
	const R parse(string str, C& ctx) const {
		str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
		return std::move(parseRaw(str, ctx));
	}

private:
	
	/**
	 * Parses a the string segment provided as a function or string literal.
	 *
	 * @param str - The string segment to parse.
	 * @param ctx - The context to parse with.
	 * @returns the parsed result.
	 */
	
	inline const R parseRaw(const std::string_view str, C& ctx) const {
		if (strIsFunction(str)) {
			let func = parseFunction(str);
			const let f = functions.find(string(func.first));
			if (f == functions.end()) {
				if (!unknownFnsAreLiteral) throw std::invalid_argument(
					"Unknown function '" + string(func.first) + "'.");
			}
			else return f->second(ctx, func.second);
		}
		
		const let& f = functions.find("_");
		if (f == functions.end()) throw std::invalid_argument("Literal specified with no literal function.");
		return f->second(ctx, { str });
	}
	
	/**
	 * Given a string segment starting and a start position pointing to an open parenthesis,
	 * finds the index of matching closing parenthesis.
	 *
	 * @param str - The string segment to search through.
	 * @param start - The position of the opening parenthesis to start at.
	 * @returns the index of the start parenthesis' closing parenthesis.
	 * @throws if the parentheses are unbalanced.
	 */
	
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
	
	/**
	 * Checks if the string segment provided is a function call.
	 *
	 * @param str - the string segment to check.
	 * @returns a boolean indicating if the string segment is a function call.
	 */
	
	const bool strIsFunction(const std::string_view& str) const {
		return str.find_first_of('(') != string::npos;
	}
	
	/**
	 * Parses a function and finds its name and parameters.
	 * Assumes that the string is a function, which should be checked beforehand with strIsFunction.
	 *
	 * @param str - The string to parse as a function.
	 * @returns a pair containing the function name, and a vector of parameter strings.
	 */
	
	const std::pair<std::string_view, vec<std::string_view>> parseFunction(const std::string_view& str) const {
		let firstParen = str.find_first_of('(');
		
		let name = str.substr(0, firstParen);
		vec<std::string_view> args {};
		
		let end = findClosingParen(str, firstParen);
		let s = firstParen + 1;
		
		while (s <= end) {
			let nextParen = str.find_first_of('(', s);
			let nextComma = str.find_first_of(',', s);
			if (nextParen != string::npos && nextParen < nextComma) nextComma = findClosingParen(str, nextParen) + 1;
			if (nextComma == string::npos) nextComma = end;
			
			args.emplace_back(str.substr(s, nextComma - s));
			s = nextComma + 1;
		}
		
		return { name, args };
	}
	
	/**
	 * Parses a string argument from an STR_ARGS.
	 *
	 * @tparam T - std::string
	 * @tparam I - The index of the parameter to parse in the args vector.
	 * @param args - An STR_ARGS to pull the parameter from.
	 * @returns the string value.
	 * @throws if the parameter cannot be interpreted as a string, or if there were not enough parameters.
	 */
	
	template <typename T, usize I, std::enable_if_t<std::is_same_v<T, string>, bool> = true>
	T parseStrArg(STR_ARGS args, C&) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return string(args[I]);
	}
	
	/**
	 * Parses an integral argument from an STR_ARGS.
	 *
	 * @tparam T - An integral type, e.g u8, i32, usize...
	 * @tparam I - The index of the parameter to parse in the args vector.
	 * @param args - An STR_ARGS to pull the parameter from.
	 * @returns the integral value.
	 * @throws if the parameter cannot be interpreted as an integer, or if there were not enough parameters.
	 */
	
	template <typename T, usize I, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	T parseStrArg(STR_ARGS args, C&) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return std::stoi(args[I].data());
	}
	
	/**
	 * Parses a floating point argument from an STR_ARGS.
	 *
	 * @tparam T - A floating point type, i.e f32 or f64.
	 * @tparam I - The index of the parameter to parse in the args vector.
	 * @param args - An STR_ARGS to pull the parameter from.
	 * @returns the floating point value.
	 * @throws if the parameter cannot be interpreted as a floating point, or if there were not enough parameters.
	 */
	
	template <typename T, usize I, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	T parseStrArg(STR_ARGS args, C&) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return std::stod(args[I].data());
	}
	
	/**
	 * Parses a Data type argument from an STR_ARGS.
	 * A parse function will have a Data type argument if it wants to accept the output from another function.
	 *
	 * @tparam T - The Data type.
	 * @tparam I - The index of the parameter to parse in the args vector.
	 * @param args - An STR_ARGS to pull the parameter from.
	 * @param ctx - The context to pass into the parse functions.
	 * @returns the Data result from whatever inner functions were executed.
	 * @throws if the parameter cannot be parsed as a Data type, or if there were not enough parameters.
	 */
	
	template <typename T, usize I, std::enable_if_t<std::is_same_v<T, R>, bool> = true>
	T parseStrArg(STR_ARGS args, C& ctx) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return parseRaw(parseStrArg<string, I>(args, ctx), ctx);
	}
	
	/**
	 * Compile-time iterates over a variant type, checking if the parameter specified
	 * matches any of the types in it. If one matches, it is parsed and returned.
	 *
	 * @tparam T - A variant type containing other parsable types.
	 * @tparam I - The index of the parameter to parse in the **args vector.**
	 * @tparam VI - The index of the variant to try to parse.
	 * @param args - An STR_ARGS to pull the parameter from.
	 * @param ctx - The context to pass into the parse functions.
	 * @returns a variant containing the first type that the parameter can be parsed as.
	 * @throws if the parameter cannot be parsed as any of the variant types.
	 */
	
	template <typename T, usize I, usize VI = 0>
	inline T parseStrVariantArg(STR_ARGS args, C& ctx) {
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
	
	/**
	 * Parses a variant argument from an STR_ARGS.
	 * The variant can contain any other parsable types.
	 *
	 * @tparam T - The variant type.
	 * @tparam I - The index of the parameter to parse in the args vector.
	 * @param args - An STR_ARGS to pull the parameter from.
	 * @param ctx - The context to pass into the parse functions.
	 * @returns a variant containing the first type that the parameter can be parsed as.
	 * @throws if the parameter cannot be parsed as any of the variant types, or if there were not enough parameters.
	 */
	
	template<typename T, usize I, std::enable_if_t<is_variant_v<T>, bool> = true>
	T parseStrArg(STR_ARGS args, C& ctx) {
		if (I >= args.size()) throw std::invalid_argument("Not enough parameters.");
		return parseStrVariantArg<T, I>(args, ctx);
	}
	
	/**
	 * Parses an optional argument from an STR_ARGS.
	 * The optional can contain any parsable type, including variants and other optionals.*
	 * * but why the hell would you do that?
	 *
	 * @tparam T - The optional type.
	 * @tparam I - The index of the parameter to parse in the args vector.
	 * @param args - An STR_ARGS to pull the parameter from.
	 * @param ctx - The context to pass into the parse function.
	 * @returns an optional that may contain a value.
	 */
	
	template<typename T, usize I, std::enable_if_t<is_optional_v<T>, bool> = true>
	T parseStrArg(STR_ARGS args, C& ctx) {
		if (I >= args.size() || args[I].empty()) return {};
		return parseStrArg<typename T::value_type, I>(args, ctx);
	}
	
	/**
	 * Compile-time iterates over a tuple of the argument types and parses them,
	 * inserting the parsed parameters into the args tuple referenced.
	 *
	 * @tparam T - The tuple type of the arguments.
	 * @tparam I - The index of the current parameter being parsed. Used for iterating.
	 * @param strArgs - The STR_ARGS storing the string parameters.
	 * @param args - The args tuple to insert the parsed arguments into.
	 * @param ctx - The context to pass into parse functions.
	 * @throws if any of the parameters cannot be parsed, see parseStrArg implementations for details.
	 */
	
	template <typename T, usize I = 0>
	inline void parseStrArgs(STR_ARGS strArgs, T& args, C& ctx) {
		std::get<I>(args) = parseStrArg<std::tuple_element_t<I, T>, I>(strArgs, ctx);
		if constexpr (I + 1 < std::tuple_size_v<T>) parseStrArgs<T, I + 1>(strArgs, args, ctx);
	}

	/** A map of parse functions, indexed by their name. */
	std::unordered_map<string, PARSE_FN> functions {};
	
	/** If true, unknown functions will be parsed as string literals, if false, an error will be thrown. */
	bool unknownFnsAreLiteral = false;
};