#ifndef _TERREATECORE_UUID_HPP__
#define _TERREATECORE_UUID_HPP__

#include <cstring>
#include <random>

#include <core/type.hpp>

namespace Terreate::Core {
using namespace Terreate::Types;

class UUID {
private:
  static u32 const sUUIDLength = 32;

private:
  i8 mUUID[sUUIDLength] = {0};
  static std::mt19937 sRandomEngine;

private:
  void generateUUID();
  UUID(i8 const *uuid);
  UUID(Str const &uuid) {
    std::memcpy(mUUID, uuid.c_str(), sizeof(i8) * sUUIDLength);
  }

public:
  UUID() { this->generateUUID(); }
  UUID(UUID const &other) {
    std::memcpy(mUUID, other.mUUID, sizeof(i8) * sUUIDLength);
  }
  UUID(UUID &&other) {
    std::memcpy(mUUID, other.mUUID, sizeof(i8) * sUUIDLength);
  }

  i8 const *raw() const { return mUUID; }

  Str toString() const;
  size_t hash() const { return std::hash<Str>{}(this->toString()); }

  bool operator==(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) == 0;
  }
  bool operator!=(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) != 0;
  }
  bool operator<(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) < 0;
  }
  bool operator>(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) > 0;
  }
  bool operator<=(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) <= 0;
  }
  bool operator>=(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) >= 0;
  }
  UUID &operator=(UUID const &other);
  UUID &operator=(UUID &&other);
  operator size_t() const { return this->hash(); }
  operator Str() const { return this->toString(); }

public:
  static UUID fromBytes(Byte const *uuid) { return UUID(uuid); }
  static UUID fromString(Str const &uuid) { return UUID(uuid); }
  static UUID empty() { return UUID(nullptr); }
  static UUID copy(UUID const &uuid) {
    return UUID::fromString(uuid.toString());
  }
};

template <typename T> inline Str toStr(T const &val) {
  Stream stream;
  stream << val;
  return stream.str();
}

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

inline ZonedTime getCurrentTime() {
  return ZonedTime{chrono::current_zone(), SystemClock::now()};
}
} // namespace Terreate::Core

std::ostream &operator<<(std::ostream &stream,
                         Terreate::Core::UUID const &uuid);

template <> struct std::hash<Terreate::Core::UUID> {
  size_t operator()(Terreate::Core::UUID const &uuid) const {
    return std::hash<std::string>()(uuid.toString());
  }
};

#endif // _TERREATECORE_UUID_HPP__
