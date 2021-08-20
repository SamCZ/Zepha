//
// Created by aurailus on 28/04/19.
//

#pragma once

#include <sstream>
#include <iostream>
#include <functional>
#include <type_traits>

#include "util/Log.h"
#include "util/Types.h"

namespace Util {
	struct EnumClassHash {
		template<typename T>
		usize operator()(T t) const {
			return static_cast<usize>(t);
		}
	};

	template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	inline static string toFixed(T val, u8 precision = 2) {
		std::ostringstream out;
		out.precision(precision);
		out << std::fixed << val;
		return out.str();
	}

	template <typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
	static string toString(T val) {
		return std::to_string(val);
	}

	template <typename T, std::enable_if_t<std::is_floating_point_v<T>, bool> = true>
	static string toString(T val) {
		return toFixed<T>(val);
	}

	template <typename V, std::enable_if_t<(std::is_trivially_copyable_v<typename V::value_type>
	    || std::is_same_v<typename V::value_type, string>)
        && std::is_same_v<vec<typename V::value_type>, V>, bool> = true>
	static string toString(V vec) {
		std::ostringstream out;
		out << "[ ";
		for (usize i = 0; i < vec.size(); i++) out << (i == 0 ? "" : ", ") << vec[i];
		out << " ]";
		return out.str();
	}

	template <typename A, std::enable_if_t<(std::is_trivially_copyable_v<typename A::value_type>
		|| std::is_same_v<typename A::value_type, string>)
		&& std::is_same_v<array<typename A::value_type, A::size_type>, A>, bool> = true>
	static string toString(A arr) {
		std::ostringstream out;
		for (usize i = 0; i < arr.size(); i++) out << (i == 0 ? "" : ", ") << arr[i];
		return out.str();
	}

	template <typename T, std::enable_if_t<std::is_integral_v<typename T::value_type> &&
		std::is_integral_v<typename T::length_type>, bool> = true>
	static string toString(T vec) {
		std::ostringstream out;
		for (usize i = 0; i < T::length(); i++) out << (i == 0 ? "" : ", ") << vec[i];
		return out.str();
	}

	template <typename T, std::enable_if_t<std::is_floating_point_v<typename T::value_type> &&
	    std::is_integral_v<typename T::length_type>, bool> = true>
	static string toString(T vec) {
		std::ostringstream out;
		for (usize i = 0; i < T::length(); i++) out << (i == 0 ? "" : ", ") << toString<typename T::value_type>(vec[i]);
		return out.str();
	}

	inline static f32 packFloat(const vec3& vec) {
		auto charX = static_cast<u8>((vec.x + 1.0f) * 0.5f * 255.f);
		auto charY = static_cast<u8>((vec.y + 1.0f) * 0.5f * 255.f);
		auto charZ = static_cast<u8>((vec.z + 1.0f) * 0.5f * 255.f);

		u32 packedInt = (charX << 16) | (charY << 8) | charZ;
		return static_cast<f32>(static_cast<f64>(packedInt) / static_cast<f64>(1 << 24));
	}

	inline static u32 intFromHexSegment(const string& t) {
		u32 x;
		std::stringstream ss;
		ss << std::hex << t;
		ss >> x;
		return x;
	}

	static vec4 hexToColorVec(string hex) {
		vec4 color {};

		if (hex[0] == '#') hex.erase(0, 1);
		else throw std::runtime_error("Color string '" + hex + "' is ill-formed. (missing #)");

		string r, g, b, a;

		if (hex.length() == 3 || hex.length() == 4) {
			r = hex.substr(0, 1);
			r += r;
			g = hex.substr(1, 1);
			g += g;
			b = hex.substr(2, 1);
			b += b;
			a = (hex.length() == 4) ? hex.substr(3, 1) : "f";
			a += a;
		}
		else if (hex.length() == 6 || hex.length() == 8) {
			r = hex.substr(0, 2);
			g = hex.substr(2, 2);
			b = hex.substr(4, 2);
			a = (hex.length() == 8) ? hex.substr(6, 2) : "ff";
		}
		else {
			throw std::runtime_error("Color string '" + hex + "' is ill-formed. (invalid length)");
		}

		color.r = intFromHexSegment(r) / 255.f;
		color.g = intFromHexSegment(g) / 255.f;
		color.b = intFromHexSegment(b) / 255.f;
		color.a = intFromHexSegment(a) / 255.f;
		
		return color;
	}

	static string getKeyStr(u16 key) {
		switch (key) {
		default: return "";

		case 0: return "mouse0";
		case 1: return "mouse1";
		case 2: return "mouse2";
		case 3: return "mouse3";
		case 4: return "mouse4";
		case 5: return "mouse5";
		case 6: return "mouse6";
		case 7: return "mouse7";
		case 8: return "scrollup";
		case 9: return "scrolldown";
		case 10: return "scrollleft";
		case 11: return "scrollright";

		case 32: return "space";
		case 39: return "'";
		case 44: return ",";
		case 45: return "-";
		case 46: return ".";
		case 47: return "/";
		case 48: return "0";
		case 49: return "1";
		case 50: return "2";
		case 51: return "3";
		case 52: return "4";
		case 53: return "5";
		case 54: return "6";
		case 55: return "7";
		case 56: return "8";
		case 57: return "9";
		case 59: return ";";
		case 61: return "=";
		case 65: return "a";
		case 66: return "b";
		case 67: return "c";
		case 68: return "d";
		case 69: return "e";
		case 70: return "f";
		case 71: return "g";
		case 72: return "h";
		case 73: return "i";
		case 74: return "j";
		case 75: return "k";
		case 76: return "l";
		case 77: return "m";
		case 78: return "n";
		case 79: return "o";
		case 80: return "p";
		case 81: return "q";
		case 82: return "r";
		case 83: return "s";
		case 84: return "t";
		case 85: return "u";
		case 86: return "v";
		case 87: return "w";
		case 88: return "x";
		case 89: return "y";
		case 90: return "z";
		case 91: return "[";
		case 92: return "\\";
		case 93: return "]";
		case 96: return "`";
		case 161: return "world1";
		case 162: return "world2";
		case 256: return "escape";
		case 257: return "enter";
		case 258: return "tab";
		case 259: return "backspace";
		case 260: return "insert";
		case 261: return "delete";
		case 262: return "right";
		case 263: return "left";
		case 264: return "down";
		case 265: return "up";
		case 266: return "pageup";
		case 267: return "pagedown";
		case 268: return "home";
		case 269: return "end";
		case 280: return "capslock";
		case 281: return "scrolllock";
		case 282: return "numlock";
		case 283: return "printscreen";
		case 284: return "pause";
		case 290: return "f1";
		case 291: return "f2";
		case 292: return "f3";
		case 293: return "f4";
		case 294: return "f5";
		case 295: return "f6";
		case 296: return "f7";
		case 297: return "f8";
		case 298: return "f9";
		case 299: return "f10";
		case 300: return "f11";
		case 301: return "f12";
		case 302: return "f13";
		case 303: return "f14";
		case 304: return "f15";
		case 305: return "f16";
		case 306: return "f17";
		case 307: return "f18";
		case 308: return "f19";
		case 309: return "f20";
		case 310: return "f21";
		case 311: return "f22";
		case 312: return "f23";
		case 313: return "f24";
		case 314: return "f25";
		case 320: return "num0";
		case 321: return "num1";
		case 322: return "num2";
		case 323: return "num3";
		case 324: return "num4";
		case 325: return "num5";
		case 326: return "num6";
		case 327: return "num7";
		case 328: return "num8";
		case 329: return "num9";
		case 330: return "num.";
		case 331: return "num/";
		case 332: return "num*";
		case 333: return "num-";
		case 334: return "num+";
		case 335: return "numenter";
		case 336: return "num=";
		case 340: return "leftshift";
		case 341: return "leftctrl";
		case 342: return "leftalt";
		case 344: return "rightshift";
		case 345: return "rightctrl";
		case 346: return "rightalt";
		case 348: return "menu";
		}
	}

	namespace {
		constexpr static u64 mix(char m, u64 s) {
			return ((s << 7) + ~(s >> 3)) + ~m;
		}
	}

	constexpr static u64 hash(const char* m) {
		return (*m) ? mix(*m, hash(m + 1)) : 0;
	}

	template<class C, typename Ret, typename ... Ts>
	std::function<Ret(Ts...)> bind_this(C* c, Ret (C::*m)(Ts...)) {
		return [=](auto&& ... args) { return (c->*m)(std::forward<decltype(args)>(args)...); };
	}

	template<class C, typename Ret, typename ... Ts>
	std::function<Ret(Ts...)> bind_this(const C* c, Ret (C::*m)(Ts...) const) {
		return [=](auto&& ... args) { return (c->*m)(std::forward<decltype(args)>(args)...); };
	}
};

template <typename T, std::enable_if_t<std::is_trivial_v<T>
    || (std::is_trivial_v<typename T::value_type> && std::is_integral_v<typename T::length_type>), bool> = true>
std::ostream& operator<<(std::ostream& out, const T& t) {
	return out << Util::toString(t);
}
