#pragma once
#include <chrono>

namespace Terreate::Type {
// Chrono types
namespace chrono = std::chrono;
typedef chrono::milliseconds msec;
typedef chrono::nanoseconds nsec;
typedef chrono::steady_clock SteadyClock;
typedef SteadyClock::time_point SteadyTimePoint;
template <typename T> using ZonedTimeT = chrono::zoned_time<T>;
using ZonedTime = ZonedTimeT<nsec>;

template <typename S, typename T> inline S durationCast(T const &time) {
  return chrono::duration_cast<S>(time);
}

inline SteadyTimePoint now() { return chrono::steady_clock::now(); }

inline SteadyClock::duration sinceEpoch() { return now().time_since_epoch(); }

inline double getNanoSec() {
  return static_cast<double>(durationCast<nsec>(sinceEpoch()).count()) /
         1000000000;
}

inline double getMilliSec() {
  return static_cast<double>(durationCast<msec>(sinceEpoch()).count()) / 1000;
}

inline ZonedTime getCurrentTime() {
  return ZonedTime{chrono::current_zone(), chrono::system_clock::now()};
}
} // namespace Terreate::Type
