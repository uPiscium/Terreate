#pragma once
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>

#define PROHIBIT_COPY_AND_ASSIGN(ClassName)                                    \
  ClassName(ClassName const &) = delete;                                       \
  ClassName &operator=(ClassName const &) = delete;

#define LOCATION_INFO()                                                        \
  std::string(__FILE__) + "/ line: " + std::to_string(__LINE__) + " " +        \
      __FUNCTION__

namespace Terreate::Type {
typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;
typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef std::string str;
typedef std::stringstream Stream;
typedef std::ifstream InputFileStream;
typedef std::ofstream OutputFileStream;

template <typename T> using optional = std::optional<T>;
template <typename T> using vec = std::vector<T>;
template <typename T> using uset = std::unordered_set<T>;
template <typename T> using set = std::set<T>;
template <typename Key, typename Value> using mmap = std::multimap<Key, Value>;
template <typename Key, typename Value>
using umap = std::unordered_map<Key, Value>;
template <typename Key, typename Value> using map = std::map<Key, Value>;
template <typename S, typename T = S> using pair = std::pair<S, T>;
template <typename T> using shared = std::shared_ptr<T>;

template <typename... Args> using function = std::function<Args...>;

template <typename T> using numlimit = std::numeric_limits<T>;

struct Version {
  u32 major;
  u32 minor;
  u32 patch;

  Version(u32 major, u32 minor, u32 patch)
      : major(major), minor(minor), patch(patch) {}
};

Version const TERREATE_VERSION = {0, 1, 0};

} // namespace Terreate::Type
