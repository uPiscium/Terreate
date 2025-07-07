#pragma once

#include <cstdint>
#include <fstream>
#include <functional>
#include <memory>
#include <sstream>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <glad/gl.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

#ifndef UNICODE_HALF_SPACE
#define UNICODE_HALF_SPACE 32
#endif // UNICODE_HALF_SPACE

#ifndef UNICODE_FULL_SPACE
#define UNICODE_FULL_SPACE 32306
#endif // UNICODE_FULL_SPACE

#ifndef PROHIBIT_COPY_AND_ASSIGN
#define PROHIBIT_COPY_AND_ASSIGN(TypeName)                                     \
  TypeName(TypeName const &) = delete;                                         \
  TypeName &operator=(TypeName const &) = delete;
#endif // PROHIBIT_COPY_AND_ASSIGN

namespace Terreate {
typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;

typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef i8 byte;
typedef u8 ubyte;

typedef std::string str;
typedef std::wstring wstr;
typedef std::stringstream stream;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;

// SDL types
typedef SDL_GUID GUID;
typedef SDL_AudioSpec AudioSpec;

// OpenGL types
typedef u32 GLObject;

template <typename S, typename T> using umap = std::unordered_map<S, T>;
template <typename S, typename T = S> using pair = std::pair<S, T>;
template <typename T> using uset = std::unordered_set<T>;
template <typename T> using vec = std::vector<T>;
template <typename T> using shared = std::shared_ptr<T>;
template <typename T> using unique = std::unique_ptr<T>;
template <typename T> using function = std::function<T>;

template <typename Derived, typename Base>
concept extends = std::derived_from<Derived, Base>;
template <typename Enum>
concept enumtype = std::is_enum_v<Enum>;
} // namespace Terreate
