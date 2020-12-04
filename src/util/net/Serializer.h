//
// Created by aurailus on 2020-01-06.
//

#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <type_traits>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Packet.h"

class Serializer {
	public:
	std::string data{};
	
	template<typename T>
	inline Serializer& append(const T& elem) { throw std::runtime_error("Tried to append a non-serializable type"); };
	
	template<typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
	inline Serializer& appendE(const E& elem) {
		append<unsigned short>(static_cast<unsigned short>(elem));
		return *this;
	}
	
	Packet packet(Packet::Type p = Packet::Type::UNDEFINED, bool reliable = true) {
		Packet packet(p, reliable);
		packet.data = data;
		return std::move(packet);
	};
	
	private:
	typedef union {
		long long ln;
		char bytes[8];
	} long_long_union;
	typedef union {
		int in;
		char bytes[4];
	} int_union;
	typedef union {
		unsigned int in;
		char bytes[4];
	} uint_union;
	typedef union {
		short sh;
		char bytes[2];
	} short_union;
	typedef union {
		unsigned short sh;
		char bytes[2];
	} ushort_union;
	typedef union {
		float fl;
		char bytes[4];
	} float_union;
};

template<>
inline Serializer& Serializer::append<long long>(const long long& elem) {
	long_long_union cv = { elem };
	data += cv.bytes[0];
	data += cv.bytes[1];
	data += cv.bytes[2];
	data += cv.bytes[3];
	data += cv.bytes[4];
	data += cv.bytes[5];
	data += cv.bytes[6];
	data += cv.bytes[7];
	return *this;
}

template<>
inline Serializer& Serializer::append<int>(const int& elem) {
	int_union cv = { elem };
	data += cv.bytes[0];
	data += cv.bytes[1];
	data += cv.bytes[2];
	data += cv.bytes[3];
	return *this;
}

template<>
inline Serializer& Serializer::append<unsigned int>(const unsigned int& elem) {
	uint_union cv = { elem };
	data += cv.bytes[0];
	data += cv.bytes[1];
	data += cv.bytes[2];
	data += cv.bytes[3];
	return *this;
}

template<>
inline Serializer& Serializer::append<short>(const short& elem) {
	short_union cv = { elem };
	data += cv.bytes[0];
	data += cv.bytes[1];
	return *this;
}

template<>
inline Serializer& Serializer::append<unsigned short>(const unsigned short& elem) {
	ushort_union cv = { elem };
	data += cv.bytes[0];
	data += cv.bytes[1];
	return *this;
}

template<>
inline Serializer& Serializer::append<char>(const char& elem) {
	data += elem;
	return *this;
}

template<>
inline Serializer& Serializer::append<unsigned char>(const unsigned char& elem) {
	data += static_cast<char>(elem);
	return *this;
}

template<>
inline Serializer& Serializer::append<bool>(const bool& elem) {
	append<char>(elem);
	return *this;
}

template<>
inline Serializer& Serializer::append<float>(const float& elem) {
	float_union cv = { elem };
	data += cv.bytes[0];
	data += cv.bytes[1];
	data += cv.bytes[2];
	data += cv.bytes[3];
	return *this;
}

template<>
inline Serializer& Serializer::append<std::string>(const std::string& elem) {
	append<unsigned int>(elem.length());
	data += elem;
	return *this;
}

template<>
inline Serializer& Serializer::append<glm::vec2>(const glm::vec2& elem) {
	append<float>(elem.x);
	append<float>(elem.y);
	return *this;
}

template<>
inline Serializer& Serializer::append<glm::ivec2>(const glm::ivec2& elem) {
	append<int>(elem.x);
	append<int>(elem.y);
	return *this;
}

template<>
inline Serializer& Serializer::append<glm::vec3>(const glm::vec3& elem) {
	append<float>(elem.x);
	append<float>(elem.y);
	append<float>(elem.z);
	return *this;
}

template<>
inline Serializer& Serializer::append<glm::ivec3>(const glm::ivec3& elem) {
	append<int>(elem.x);
	append<int>(elem.y);
	append<int>(elem.z);
	return *this;
}

template<>
inline Serializer& Serializer::append<std::vector<int>>(const std::vector<int>& elem) {
	data.reserve(data.length() + elem.size() * 4 + 4);
	append<unsigned int>(elem.size());
	data += std::string{ reinterpret_cast<const char*>(&elem[0]), elem.size() * 4 };
	return *this;
}

template<>
inline Serializer& Serializer::append<std::vector<unsigned int>>(const std::vector<unsigned int>& elem) {
	data.reserve(data.length() + elem.size() * 4 + 4);
	append<unsigned int>(elem.size());
	data += std::string{ reinterpret_cast<const char*>(&elem[0]), elem.size() * 4 };
	return *this;
}

template<>
inline Serializer& Serializer::append<std::vector<short>>(const std::vector<short>& elem) {
	data.reserve(data.length() + elem.size() * 2 + 4);
	append<unsigned int>(elem.size());
	data += std::string{ reinterpret_cast<const char*>(&elem[0]), elem.size() * 2 };
	return *this;
}

template<>
inline Serializer& Serializer::append<std::vector<unsigned short>>(const std::vector<unsigned short>& elem) {
	data.reserve(data.length() + elem.size() * 2 + 4);
	append<unsigned int>(elem.size());
	data += std::string{ reinterpret_cast<const char*>(&elem[0]), elem.size() * 2 };
	return *this;
}

template<>
inline Serializer& Serializer::append<std::vector<char>>(const std::vector<char>& elem) {
	data.reserve(data.length() + elem.size() + 4);
	append<unsigned int>(elem.size());
	data += std::string{ reinterpret_cast<const char*>(&elem[0]), elem.size() };
	return *this;
}

template<>
inline Serializer& Serializer::append<std::vector<unsigned char>>(const std::vector<unsigned char>& elem) {
	data.reserve(data.length() + elem.size() + 4);
	append<unsigned int>(elem.size());
	data += std::string{ reinterpret_cast<const char*>(&elem[0]), elem.size() };
	return *this;
}

template<>
inline Serializer& Serializer::append<std::vector<float>>(const std::vector<float>& elem) {
	data.reserve(data.length() + elem.size() * 4 + 4);
	append<unsigned int>(elem.size());
	data += std::string{ reinterpret_cast<const char*>(&elem[0]), elem.size() * 4 };
	return *this;
}

template<>
inline Serializer& Serializer::append<std::vector<std::string>>(const std::vector<std::string>& elem) {
	append<unsigned int>(elem.size());
	for (unsigned int i = 0; i < elem.size(); i++) {
		append<std::string>(elem[i]);
	}
	return *this;
}