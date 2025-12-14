#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

#ifndef STB_IMAGE_IMPLEMENTATION // Avoid multiple definition error
#define STB_IMAGE_IMPLEMENTATION
#else
#undef STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

// #ifndef UNICODE_HALF_SPACE
// #define UNICODE_HALF_SPACE 32
// #endif // UNICODE_HALF_SPACE

// #ifndef UNICODE_FULL_SPACE
// #define UNICODE_FULL_SPACE 32306
// #endif // UNICODE_FULL_SPACE

#ifndef PROHIBIT_COPY_AND_ASSIGN
#define PROHIBIT_COPY_AND_ASSIGN(TypeName)                                     \
  TypeName(TypeName const &) = delete;                                         \
  TypeName &operator=(TypeName const &) = delete
#endif // PROHIBIT_COPY_AND_ASSIGN

namespace Terreate {
using i8 = std::int8_t;
using i16 = std::int16_t;
using i32 = std::int32_t;
using i64 = std::int64_t;

using u8 = std::uint8_t;
using u16 = std::uint16_t;
using u32 = std::uint32_t;
using u64 = std::uint64_t;

using byte = i8;
using ubyte = u8;

using AudioDeviceID = SDL_AudioDeviceID;
using CameraID = SDL_CameraID;
using JoystickID = SDL_JoystickID;
using MouseID = SDL_MouseID;
using WindowID = SDL_WindowID;
using DisplayID = SDL_DisplayID;
using KeyboardID = SDL_KeyboardID;
using SensorID = SDL_SensorID;
using PenID = SDL_PenID;

using str = std::string;
using wstr = std::wstring;
using stream = std::stringstream;
using ifstream = std::ifstream;
using ofstream = std::ofstream;
using mutex = std::mutex;

using typeinfo = std::type_info;
using typeindex = std::type_index;

// SDL types
using GUID = SDL_GUID;
using AudioSpec = SDL_AudioSpec;

// Vulkan types
using Rect2D = VkRect2D;
using Viewport = VkViewport;

template <typename S, typename T, typename Hash = std::hash<S>>
using umap = std::unordered_map<S, T, Hash>;
template <typename S, typename T = S> using pair = std::pair<S, T>;
template <typename T> using uset = std::unordered_set<T>;
template <typename T, u64 N> using array = std::array<T, N>;
template <typename T> using vec = std::vector<T>;
template <typename T> using shared = std::shared_ptr<T>;
template <typename T> using unique = std::unique_ptr<T>;
template <typename T> using function = std::function<T>;
template <typename T> using guard = std::lock_guard<T>;
template <typename T> using ulock = std::unique_lock<T>;

template <typename... Ts> using variant = std::variant<Ts...>;

template <typename Enum>
concept enumtype = std::is_enum_v<Enum>;

static constexpr u32 SDL_FLAGS =
    SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMEPAD |
    SDL_INIT_HAPTIC | SDL_INIT_CAMERA | SDL_INIT_SENSOR | SDL_INIT_AUDIO;

struct Version {
  u32 major;
  u32 minor;
  u32 patch;
};

u32 constexpr makeVersion(Version const &version) {
  return (version.major << 22) | (version.minor << 12) | (version.patch);
}

} // namespace Terreate
