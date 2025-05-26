#pragma once
#include "../decl/type.hpp"

#include <cstring>
#include <random>

namespace Terreate::Util {
class UUID {
private:
  static unsigned const sUUIDLength = 32;

private:
  Type::i8 mUUID[sUUIDLength] = {0};
  static std::mt19937 sRandomEngine;

private:
  void GenerateUUID();
  UUID(Type::i8 const *uuid);
  UUID(Type::str const &uuid) {
    std::memcpy(mUUID, uuid.c_str(), sizeof(Type::i8) * sUUIDLength);
  }

public:
  UUID() { this->GenerateUUID(); }
  UUID(Util::UUID const &other) {
    std::memcpy(mUUID, other.mUUID, sizeof(Type::i8) * sUUIDLength);
  }
  UUID(Util::UUID &&other) {
    std::memcpy(mUUID, other.mUUID, sizeof(Type::i8) * sUUIDLength);
  }

  Type::i8 const *Raw() const { return mUUID; }

  Type::str toString() const;
  size_t hash() const { return std::hash<Type::str>{}(this->toString()); }

  bool operator==(Util::UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(Type::i8) * sUUIDLength) == 0;
  }
  bool operator!=(Util::UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(Type::i8) * sUUIDLength) != 0;
  }
  bool operator<(Util::UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(Type::i8) * sUUIDLength) < 0;
  }
  bool operator>(Util::UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(Type::i8) * sUUIDLength) > 0;
  }
  bool operator<=(Util::UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(Type::i8) * sUUIDLength) <= 0;
  }
  bool operator>=(Util::UUID const &other) const {
    return std::memcmp(mUUID, other.mUUID, sizeof(Type::i8) * sUUIDLength) >= 0;
  }
  Util::UUID &operator=(Util::UUID const &other);
  Util::UUID &operator=(Util::UUID &&other);
  operator size_t() const { return this->hash(); }
  operator Type::str() const { return this->toString(); }

public:
  static Util::UUID fromTCi8(Type::i8 const *uuid) { return UUID(uuid); }
  static Util::UUID fromString(Type::str const &uuid) { return UUID(uuid); }
  static Util::UUID empty() { return UUID(nullptr); }
  static Util::UUID copy(Util::UUID const &uuid) {
    return UUID::fromString(uuid.toString());
  }
};

} // namespace Terreate::Util

std::ostream &operator<<(std::ostream &stream,
                         Terreate::Util::UUID const &uuid);

template <> struct std::hash<Terreate::Util::UUID> {
  size_t operator()(Terreate::Util::UUID const &uuid) const {
    return std::hash<std::string>()(uuid.toString());
  }
};
