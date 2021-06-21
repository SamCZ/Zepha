//
// Created by aurailus on 2020-01-06.
//

#pragma once

#include <array>
#include <string>
#include <vector>
#include <stdexcept>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Deserializer {
public:
	Deserializer(const std::string& data) : data(&data[0]), len(data.length()) {};
	
	Deserializer(const char* start, size_t len) : data(start), len(len) {};
	
	template<typename T>
	inline T read() { throw std::runtime_error("Tried to append a non-serializable type"); };
	
	template<typename T>
	inline Deserializer& read(T& ref) {
		ref = read<T>();
		return *this;
	};
	
	template<typename T, int L>
	inline std::array<T, L> readArr() {
		auto oldInd = ind;
		ind += L * sizeof(T);
		
		std::array<T, L> res;
		for (int i = 0; i < L; i++) res[i] = *reinterpret_cast<const T*>(&data[oldInd + i * sizeof(T)]);
		return res;
	}
	
	template<typename T, int L>
	inline Deserializer& readArr(std::array<T, L>& ref) {
		ref = readArr<T, L>();
		return *this;
	}
	
	template<typename T>
	inline std::vector<T> readVec() {
		unsigned int len = read<unsigned int>();
		auto oldInd = ind;
		ind += len * sizeof(T);
		return std::vector<T>(
			reinterpret_cast<const T*>(&data[oldInd]),
			reinterpret_cast<const T*>(&data[ind]));
	}
	
	template<typename T>
	inline Deserializer& readVec(std::vector<T>& ref) {
		ref = readVec<T>();
		return *this;
	}
	
	template<typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
	inline E readEnum() {
		return static_cast<E>(read<unsigned short>());
	};
	
	template<typename E, typename = typename std::enable_if<std::is_enum<E>::value>::type>
	inline Deserializer& readEnum(E& ref) {
		ref = static_cast<E>(read<unsigned short>());
		return *this;
	};
	
	bool atEnd() {
		return ind >= len;
	};
	
	const char* data;
	size_t len;
	size_t ind = 0;
	
private:
	typedef union {
		int ln;
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
inline long long Deserializer::read<long long>() {
	long_long_union cv;
	cv.bytes[0] = data[ind];
	cv.bytes[1] = data[ind + 1];
	cv.bytes[2] = data[ind + 2];
	cv.bytes[3] = data[ind + 3];
	cv.bytes[4] = data[ind + 4];
	cv.bytes[5] = data[ind + 5];
	cv.bytes[6] = data[ind + 6];
	cv.bytes[7] = data[ind + 7];
	ind += 8;
	return cv.ln;
}

template<>
inline int Deserializer::read<int>() {
	int_union cv;
	cv.bytes[0] = data[ind];
	cv.bytes[1] = data[ind + 1];
	cv.bytes[2] = data[ind + 2];
	cv.bytes[3] = data[ind + 3];
	ind += 4;
	return cv.in;
}

template<>
inline unsigned int Deserializer::read<unsigned int>() {
	uint_union cv;
	cv.bytes[0] = data[ind];
	cv.bytes[1] = data[ind + 1];
	cv.bytes[2] = data[ind + 2];
	cv.bytes[3] = data[ind + 3];
	ind += 4;
	return cv.in;
}

template<>
inline short Deserializer::read<short>() {
	short_union cv;
	cv.bytes[0] = data[ind];
	cv.bytes[1] = data[ind + 1];
	ind += 2;
	return cv.sh;
}

template<>
inline unsigned short Deserializer::read<unsigned short>() {
	ushort_union cv;
	cv.bytes[0] = data[ind];
	cv.bytes[1] = data[ind + 1];
	ind += 2;
	return cv.sh;
}

template<>
inline char Deserializer::read<char>() {
	return data[ind++];
}

template<>
inline unsigned char Deserializer::read<unsigned char>() {
	return static_cast<unsigned char>(data[ind++]);
}

template<>
inline bool Deserializer::read<bool>() {
	return read<char>();
}

template<>
inline float Deserializer::read<float>() {
	float_union cv;
	cv.bytes[0] = data[ind];
	cv.bytes[1] = data[ind + 1];
	cv.bytes[2] = data[ind + 2];
	cv.bytes[3] = data[ind + 3];
	ind += 4;
	return cv.fl;
}

template<>
inline std::string Deserializer::read<std::string>() {
	unsigned int len = read<unsigned int>();
	size_t i = ind;
	ind += len;
	return std::string(&data[i], len);
}

template<>
inline glm::vec2 Deserializer::read<glm::vec2>() {
	return {
		read<float>(),
		read<float>()
	};
}

template<>
inline glm::ivec2 Deserializer::read<glm::ivec2>() {
	return {
		read<int>(),
		read<int>()
	};
}

template<>
inline glm::vec3 Deserializer::read<glm::vec3>() {
	return {
		read<float>(),
		read<float>(),
		read<float>()
	};
}

template<>
inline glm::ivec3 Deserializer::read<glm::ivec3>() {
	return {
		read<int>(),
		read<int>(),
		read<int>()
	};
}