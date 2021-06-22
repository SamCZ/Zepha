#pragma once

#include <array>
#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <type_traits>

#include "util/Types.h"

class Deserializer {
public:
	Deserializer(const string& data) : data(&data[0]), len(data.length()) {};
	Deserializer(const char* start, usize len) : data(start), len(len) {};
	
	/** Reads a value into a ref of it, returning the Deserializer for chaining. */
	template<typename T>
	inline Deserializer& read(T& into) {
		into = read<T>();
		return *this;
	}
	
	/** Reads a trivial value. */
	template<typename T, std::enable_if_t<std::is_trivially_copyable_v<T> && !std::is_enum_v<T>, bool> = true>
	inline T read() {
		T value;
		std::memcpy(&value, &data[ind], sizeof(T));
		ind += sizeof(T);
		return value;
	}

	/** Reads a string. */
	template<typename T, std::enable_if_t<std::is_same_v<string, T>, bool> = true>
	inline string read() {
		usize stringLen = read<usize>();
		ind += stringLen;
		return string(&data[ind - stringLen], stringLen);
	}

	/** Reads an array of trivial values. */
	template<typename A,
		std::enable_if_t<std::is_trivially_copyable_v<typename A::value_type> &&
		std::is_same_v<array<typename A::value_type, A::size_type>, A>, bool> = true>
	inline A read() {
		usize startInd = ind;
		ind += A::size_type * sizeof(typename A::value_type);
		std::array<typename A::value_type, A::size_type> res;
		for (int i = 0; i < A::size_type; i++) res[i] = *reinterpret_cast<const typename A::value_type*>(
			&data[startInd + i * sizeof(A::value_type)]);
		return res;
	}
	
	/** Reads a vector of trivial values. */
	template<typename V,
		std::enable_if_t<std::is_trivially_copyable_v<typename V::value_type> &&
		std::is_same_v<vec<typename V::value_type>, V>, bool> = true>
	inline V read() {
		usize vecLen = read<usize>();
		auto startInd = ind;
		ind += vecLen * sizeof(typename V::value_type);
		return vec<typename V::value_type>(
			reinterpret_cast<const typename V::value_type*>(&data[startInd]),
			reinterpret_cast<const typename V::value_type*>(&data[ind]));
	}
	
	/** Reads a vector of strings. */
	template<typename V,
		std::enable_if_t<std::is_same_v<string, typename V::value_type> &&
		std::is_same_v<vec<typename V::value_type>, V>, bool> = true>
	inline V read() {
		usize vecLen = read<usize>();
		V dataVec = {};
		dataVec.reserve(vecLen);
		for (usize i = 0; i < vecLen; i++) dataVec.emplace_back(read<typename V::value_type>());
		return dataVec;
	}

	/** Reads an enum. */
	template<typename E, std::enable_if_t<std::is_enum_v<E>, bool> = true>
	inline E read() {
		return static_cast<E>(read<u16>());
	};
	
	/** Returns true if there's no more data to be read. */
	bool atEnd() {
		return ind >= len;
	};
	
	/** A reference to the data. */
	const char* data;
	
	/** The length of the data. */
	usize len;
	
	/** The current index. */
	usize ind = 0;
};