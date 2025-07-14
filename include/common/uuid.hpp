#pragma once

#include <cstring>
#include <random>

#include "type.hpp"

namespace Terreate {
class UUID {
private:
  static u32 const sUUIDLength = 32;

private:
  i8 mUUID[sUUIDLength] = {0};
  static std::mt19937 sRandomEngine;

private:
  void generateUUID();
  UUID(i8 const *uuid);
  UUID(str const &uuid) {
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

  i8 const *Raw() const { return mUUID; }

  str toString() const;
  size_t hash() const { return std::hash<str>{}(this->toString()); }

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
  operator str() const { return this->toString(); }

public:
  static UUID fromi8(i8 const *uuid) { return UUID(uuid); }
  static UUID fromString(str const &uuid) { return UUID(uuid); }
  static UUID empty() { return UUID(nullptr); }
  static UUID copy(UUID const &uuid) {
    return UUID::fromString(uuid.toString());
  }
};

template <typename T> inline str toStr(T const &val) {
  stream buf;
  buf << val;
  return buf.str();
}
} // namespace Terreate

std::ostream &operator<<(std::ostream &stream, Terreate::UUID const &uuid);

template <> struct std::hash<Terreate::UUID> {
  size_t operator()(Terreate::UUID const &uuid) const {
    return std::hash<std::string>()(uuid.toString());
  }
};
