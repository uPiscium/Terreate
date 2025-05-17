#ifndef __TERREATE_TYPES_HPP__
#define __TERREATE_TYPES_HPP__

#include <chrono>
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

#ifndef TC_UNICODE_HALF_SPACE
#define TC_UNICODE_HALF_SPACE 32
#endif // TC_UNICODE_HALF_SPACE

#ifndef TC_UNICODE_FULL_SPACE
#define TC_UNICODE_FULL_SPACE 32306
#endif // TC_UNICODE_FULL_SPACE

namespace Terreate::Types {
// Type aliases
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;

// Standard types
typedef i8 Byte;
typedef u8 Ubyte;
typedef u32 ID;
typedef u64 Index;
typedef u64 EventID;
typedef std::string Str;
typedef std::wstring WStr;
typedef std::stringstream Stream;
typedef std::ifstream InputFileStream;
typedef std::ofstream OutputFileStream;

template <typename S, typename T> using Map = std::unordered_map<S, T>;
template <typename S, typename T = S> using Pair = std::pair<S, T>;
template <typename T> using Set = std::unordered_set<T>;
template <typename T> using Vec = std::vector<T>;
template <typename T> using Shared = std::shared_ptr<T>;
template <typename T> using Unique = std::unique_ptr<T>;
template <typename T> using Function = std::function<T>;

// Chrono types
namespace chrono = std::chrono;
typedef chrono::milliseconds MilliSec;
typedef chrono::nanoseconds NanoSec;
typedef chrono::system_clock SystemClock;
typedef chrono::system_clock::time_point SystemTimePoint;
typedef chrono::steady_clock SteadyClock;
typedef chrono::steady_clock::time_point SteadyTimePoint;
template <typename T> using ZonedTimeT = chrono::zoned_time<T>;
using ZonedTime = ZonedTimeT<NanoSec>;

// Concepts
template <typename Derived, typename Base>
concept extends = std::derived_from<Derived, Base>;
template <typename Enum>
concept enumtype = std::is_enum_v<Enum>;

} // namespace Terreate::Types

#endif // __TERREATE_TYPES_HPP__
