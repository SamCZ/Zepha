/** Type aliases for integer types, vectors, and smart pointers. */

#pragma once
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedTypeAliasInspection"

#define let auto

#include <vector>
#include <string>
#include <memory>
#include <cstdint>
#include <glm/glm.hpp>

using i8  = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using i8f = int_fast8_t;
using i16f = int_fast16_t;
using i32f = int_fast32_t;
using i64f = int_fast64_t;

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;
using usize = size_t;

using u8f = uint_fast8_t;
using u16f = uint_fast16_t;
using u32f = uint_fast32_t;
using u64f = uint_fast64_t;

using f32 = float;
using f64 = double;

using vec2 = glm::f32vec2;
using vec3 = glm::f32vec3;
using vec4 = glm::f32vec4;

using glm::i8vec2;
using glm::i8vec3;
using glm::i8vec4;
using glm::i16vec2;
using glm::i16vec3;
using glm::i16vec4;
using ivec2 = glm::i32vec2;
using ivec3 = glm::i32vec3;
using ivec4 = glm::i32vec4;

using glm::u8vec2;
using glm::u8vec3;
using glm::u8vec4;
using glm::u16vec2;
using glm::u16vec3;
using glm::u16vec4;
using uvec2 = glm::u32vec2;
using uvec3 = glm::u32vec3;
using uvec4 = glm::u32vec4;

using std::array;
using std::string;

template <typename T>
using vec = std::vector<T>;

template <typename T>
using sptr = std::shared_ptr<T>;
template <typename T>
using uptr = std::unique_ptr<T>;

using std::make_shared;
using std::make_unique;

#pragma clang diagnostic pop
