#pragma once

#include <array>
#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <type_traits>

#include "Packet.h"
#include "util/Types.h"

class Serializer {
public:
	string data {};

	/** Appends a trivial value. */
	template<typename T, std::enable_if_t<std::is_trivially_copyable_v<T> && !std::is_enum_v<T>, bool> = true>
	inline Serializer& append(const T elem) {
		data.reserve(data.length() + sizeof(T));
		char c[sizeof(T)];
		std::memcpy(c, &elem, sizeof(T));
		data += std::string_view(c, sizeof(T));
		return *this;
	}
	
	/** Appends a string. */
	template<typename T, std::enable_if_t<std::is_same_v<string, T>, bool> = true>
	inline Serializer& append(const T& elem) {
		data.reserve(data.length() + sizeof(usize) + elem.size());
		append<usize>(elem.size());
		data += elem;
		return *this;
	}
	
	/** Appends an array of trivial values. */
	template<typename A,
		std::enable_if_t<std::is_trivially_copyable_v<typename A::value_type> &&
		std::is_same_v<array<typename A::value_type, A::size_type>, A>, bool> = true>
	inline Serializer& append(const A& elem) {
		data.reserve(data.length() + elem.size() * sizeof(typename A::value_type));
		data += string { reinterpret_cast<const char*>(&elem[0]), elem.size() * sizeof(A::value_type) };
		return *this;
	}
	
	/** Appends a vector of trivial values. */
	template<typename V,
		std::enable_if_t<std::is_trivially_copyable_v<typename V::value_type> &&
		std::is_same<vec<typename V::value_type>, V>::value, bool> = true>
	inline Serializer& append(const V& elem) {
		data.reserve(data.length() + elem.size() * sizeof(typename V::value_type) + sizeof(usize));
		append<usize>(elem.size());
		data += string { reinterpret_cast<const char*>(&elem[0]), elem.size() * sizeof(typename V::value_type) };
		return *this;
	}
	
	/** Appends a vector of strings. */
	template<typename V,
		std::enable_if_t<std::is_same_v<string, typename V::value_type> &&
		std::is_same<vec<typename V::value_type>, V>::value, bool> = true>
	inline Serializer& append(const V& elem) {
		append<usize>(elem.size());
		for (usize i = 0; i < elem.size(); i++) append<typename V::value_type>(elem[i]);
		return *this;
	}
	
	/** Appends an enum. */
	template<typename E, std::enable_if_t<std::is_enum_v<E>, bool> = true>
	inline Serializer& append(const E& elem) {
		append<u16>(static_cast<u16>(elem));
		return *this;
	}
	
	/** Creates a packet from the data supplied. */
	Packet packet(Packet::Type p = Packet::Type::UNDEFINED, bool reliable = true) {
		Packet packet(p, reliable);
		packet.data = data;
		return std::move(packet);
	};
};