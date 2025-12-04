#pragma once
#include <chrono>

namespace Terreate {
namespace chrono = std::chrono;
typedef chrono::milliseconds MilliSec;
typedef chrono::nanoseconds NanoSec;
typedef chrono::system_clock SystemClock;
typedef chrono::system_clock::time_point SystemTimePoint;
typedef chrono::steady_clock SteadyClock;
typedef chrono::steady_clock::time_point SteadyTimePoint;
template <typename T> using ZonedTimeT = chrono::zoned_time<T>;
using ZonedTime = ZonedTimeT<NanoSec>;

template <typename S, typename T> inline S durationCast(T const &time) {
  return chrono::duration_cast<S>(time);
}

inline SteadyTimePoint now() { return SteadyClock::now(); }

inline SteadyClock::duration sinceEpoch() { return now().time_since_epoch(); }

inline double getNanoSec() {
  return static_cast<double>(durationCast<NanoSec>(sinceEpoch()).count()) /
         1000000000;
}

inline double getMilliSec() {
  return static_cast<double>(durationCast<MilliSec>(sinceEpoch()).count()) /
         1000;
}

inline ZonedTime geurrentTime() {
  return ZonedTime{chrono::current_zone(), SystemClock::now()};
}
} // namespace Terreate
