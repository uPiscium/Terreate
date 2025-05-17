// #pragma once

// #include <cstring>
// #include <random>

// #include <type.hpp>

// namespace Terreate::Core {
// using namespace Terreate::Type;

// class UUID {
// private:
//   static unsigned const sUUIDLength = 32;

// private:
//   i8 mUUID[sUUIDLength] = {0};
//   static std::mt19937 sRandomEngine;

// private:
//   void GenerateUUID();
//   UUID(i8 const *uuid);
//   UUID(str const &uuid) {
//     std::memcpy(mUUID, uuid.c_str(), sizeof(i8) * sUUIDLength);
//   }

// public:
//   UUID() { this->GenerateUUID(); }
//   UUID(UUID const &other) {
//     std::memcpy(mUUID, other.mUUID, sizeof(i8) * sUUIDLength);
//   }
//   UUID(UUID &&other) {
//     std::memcpy(mUUID, other.mUUID, sizeof(i8) * sUUIDLength);
//   }

//   i8 const *Raw() const { return mUUID; }

//   str toString() const;
//   size_t Hash() const { return std::hash<str>{}(this->toString()); }

//   bool operator==(UUID const &other) const {
//     return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) == 0;
//   }
//   bool operator!=(UUID const &other) const {
//     return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) != 0;
//   }
//   bool operator<(UUID const &other) const {
//     return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) < 0;
//   }
//   bool operator>(UUID const &other) const {
//     return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) > 0;
//   }
//   bool operator<=(UUID const &other) const {
//     return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) <= 0;
//   }
//   bool operator>=(UUID const &other) const {
//     return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) >= 0;
//   }
//   UUID &operator=(UUID const &other);
//   UUID &operator=(UUID &&other);
//   operator size_t() const { return this->Hash(); }
//   operator str() const { return this->toString(); }

// public:
//   static UUID fromTCi8(i8 const *uuid) { return UUID(uuid); }
//   static UUID fromString(str const &uuid) { return UUID(uuid); }
//   static UUID empty() { return UUID(nullptr); }
//   static UUID copy(UUID const &uuid) {
//     return UUID::fromString(uuid.toString());
//   }
// };

// } // namespace Terreate::Core

// std::ostream &operator<<(std::ostream &stream,
//                          Terreate::Core::UUID const &uuid);

// template <> struct std::hash<Terreate::Core::UUID> {
//   size_t operator()(Terreate::Core::UUID const &uuid) const {
//     return std::hash<std::string>()(uuid.toString());
//   }
// };
