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

#ifndef UNICODE_HALF_SPACE
#define UNICODE_HALF_SPACE 32
#endif // UNICODE_HALF_SPACE

#ifndef UNICODE_FULL_SPACE
#define UNICODE_FULL_SPACE 32306
#endif // UNICODE_FULL_SPACE

#ifndef PROHIBIT_COPY_AND_ASSIGN
#define PROHIBIT_COPY_AND_ASSIGN(TypeName)                                     \
  TypeName(TypeName const &) = delete;                                         \
  TypeName &operator=(TypeName const &) = delete
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

typedef SDL_AudioDeviceID AudioDeviceID;
typedef SDL_CameraID CameraID;
typedef SDL_JoystickID JoystickID;
typedef SDL_MouseID MouseID;
typedef SDL_WindowID WindowID;
typedef SDL_DisplayID DisplayID;
typedef SDL_KeyboardID KeyboardID;
typedef SDL_SensorID SensorID;
typedef SDL_PenID PenID;

typedef std::string str;
typedef std::wstring wstr;
typedef std::stringstream stream;
typedef std::ifstream ifstream;
typedef std::ofstream ofstream;
typedef std::mutex mutex;

typedef std::type_info typeinfo;
typedef std::type_index typeindex;

// SDL types
typedef SDL_GUID GUID;
typedef SDL_AudioSpec AudioSpec;

// Vulkan types
typedef VkRect2D Rect2D;
typedef VkViewport Viewport;

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
