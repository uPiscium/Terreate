#pragma once

#include <cstring>
#include <random>

#include "common/type.hpp"

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
  UUID(str const &uuid);

public:
  UUID();
  UUID(UUID const &other);
  UUID(UUID &&other);

  i8 const *raw() const;

  str toString() const;
  size_t hash() const;

  bool operator==(UUID const &other) const;
  bool operator!=(UUID const &other) const;
  bool operator<(UUID const &other) const;
  bool operator>(UUID const &other) const;
  bool operator<=(UUID const &other) const;
  bool operator>=(UUID const &other) const;
  UUID &operator=(UUID const &other);
  UUID &operator=(UUID &&other);
  operator size_t() const;
  operator str() const;

public:
  static UUID fromi8(i8 const *uuid);
  static UUID fromString(str const &uuid);
  static UUID empty();
  static UUID copy(UUID const &uuid);
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
