#pragma once
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>

#define PROHIBIT_COPY_AND_ASSIGN(ClassName)                                    \
  ClassName(const ClassName &) = delete;                                       \
  ClassName &operator=(const ClassName &) = delete;

namespace Terreate::Type {
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
typedef std::stringstream Stream;

template <typename T> using optional = std::optional<T>;
template <typename T> using vec = std::vector<T>;
template <typename T> using set = std::set<T>;
template <typename T> using unique = std::unique_ptr<T>;
template <typename T> using shared = std::shared_ptr<T>;
template <typename S, typename T = S> using pair = std::pair<S, T>;

template <typename K, typename V> using map = std::map<K, V>;
template <typename K, typename V> using umap = std::unordered_map<K, V>;

template <typename... Args> using function = std::function<Args...>;

template <typename T> str getTypeName() { return typeid(T).name(); }

struct Version {
  int major;
  int minor;
  int patch;

  Version(int major, int minor, int patch)
      : major(major), minor(minor), patch(patch) {}
  operator str() const {
    return std::to_string(major) + "." + std::to_string(minor) + "." +
           std::to_string(patch);
  }
  operator u32() const { return (major << 22) | (minor << 12) | patch; }
};

inline Version TERREATE_ENGINE_VERSION = Version(0, 1, 0);
} // namespace Terreate::Type
