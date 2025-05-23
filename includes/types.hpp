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
// TC types (TerreateCore types)
typedef bool TCbool;
typedef int8_t TCi8;
typedef uint8_t TCu8;
typedef int16_t TCi16;
typedef uint16_t TCu16;
typedef int32_t TCi32;
typedef uint32_t TCu32;
typedef int64_t TCi64;
typedef uint64_t TCu64;
typedef float TCfloat;
typedef double TCdouble;

// Standard types
typedef TCbool Bool;
typedef TCi8 Byte;
typedef TCu8 Ubyte;
typedef TCi16 Short;
typedef TCu16 Ushort;
typedef TCi32 Int;
typedef TCu32 Uint;
typedef TCi64 Long;
typedef TCu64 Ulong;
typedef TCfloat Float;
typedef TCdouble Double;

typedef TCu32 ID;
typedef TCu64 Index;
typedef TCu64 EventID;
typedef TCu64 Size;
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
