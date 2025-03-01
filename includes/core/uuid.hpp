#ifndef _TERREATECORE_UUID_HPP__
#define _TERREATECORE_UUID_HPP__

#include <cstring>
#include <random>

#include <types.hpp>

namespace Terreate::Core {
using namespace Terreate::Types;

class UUID {
private:
  static Uint const sUUIDLength = 32;

private:
  TCi8 mUUID[sUUIDLength] = {0};
  static std::mt19937 sRandomEngine;

private:
  void GenerateUUID();
  UUID(TCi8 const *uuid);
  UUID(Str const &uuid) {
    std::memcpy(mUUID, uuid.c_str(), sizeof(TCi8) * sUUIDLength);
  }

public:
  UUID() { this->GenerateUUID(); }
  UUID(UUID const &other) {
    std::memcpy(mUUID, other.mUUID, sizeof(TCi8) * sUUIDLength);
  }
  UUID(UUID &&other) {
    std::memcpy(mUUID, other.mUUID, sizeof(TCi8) * sUUIDLength);
  }

  TCi8 const *Raw() const { return mUUID; }

  Str ToString() const;
  size_t Hash() const { return std::hash<Str>{}(this->ToString()); }

  bool operator==(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(TCi8) * sUUIDLength) == 0;
  }
  bool operator!=(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(TCi8) * sUUIDLength) != 0;
  }
  bool operator<(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(TCi8) * sUUIDLength) < 0;
  }
  bool operator>(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(TCi8) * sUUIDLength) > 0;
  }
  bool operator<=(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(TCi8) * sUUIDLength) <= 0;
  }
  bool operator>=(UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(TCi8) * sUUIDLength) >= 0;
  }
  UUID &operator=(UUID const &other);
  UUID &operator=(UUID &&other);
  operator size_t() const { return this->Hash(); }
  operator Str() const { return this->ToString(); }

public:
  static UUID FromTCi8(TCi8 const *uuid) { return UUID(uuid); }
  static UUID FromString(Str const &uuid) { return UUID(uuid); }
  static UUID Empty() { return UUID(nullptr); }
  static UUID Copy(UUID const &uuid) {
    return UUID::FromString(uuid.ToString());
  }
};

template <typename T> inline Str ToStr(T const &val) {
  Stream stream;
  stream << val;
  return stream.str();
}

template <typename S, typename T> inline S DurationCast(T const &time) {
  return chrono::duration_cast<S>(time);
}

inline SteadyTimePoint Now() { return SteadyClock::now(); }

inline SteadyClock::duration SinceEpoch() { return Now().time_since_epoch(); }

inline Double GetNanoSec() {
  return static_cast<Double>(DurationCast<NanoSec>(SinceEpoch()).count()) /
         1000000000;
}

inline Double GetMilliSec() {
  return static_cast<Double>(DurationCast<MilliSec>(SinceEpoch()).count()) /
         1000;
}

inline ZonedTime GetCurrentTime() {
  return ZonedTime{chrono::current_zone(), SystemClock::now()};
}
} // namespace Terreate::Core

std::ostream &operator<<(std::ostream &stream,
                         Terreate::Core::UUID const &uuid);

template <> struct std::hash<Terreate::Core::UUID> {
  size_t operator()(Terreate::Core::UUID const &uuid) const {
    return std::hash<std::string>()(uuid.ToString());
  }
};

#endif // _TERREATECORE_UUID_HPP__
