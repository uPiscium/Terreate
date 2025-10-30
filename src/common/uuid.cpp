#include "common/time.hpp"
#include "common/uuid.hpp"

namespace Terreate {
std::mt19937 UUID::sRandomEngine = std::mt19937(std::random_device()());

void UUID::generateUUID() {
  time_t epoch = sinceEpoch().count();
  u16 const *epochBlock = reinterpret_cast<u16 const *>(&epoch);
  for (int i = 0; i < sizeof(time_t) / sizeof(u16); ++i) {
    std::memcpy(&mUUID[i * sizeof(u16)],
                &epochBlock[(sizeof(time_t) / sizeof(u16)) - i - 1],
                sizeof(u16));
  }
  u32 randomLength = (sUUIDLength - sizeof(time_t)) / sizeof(u32);
  for (int i = 0; i < randomLength; ++i) {
    u32 random = sRandomEngine();
    std::memcpy(&mUUID[sizeof(time_t) + i * sizeof(u32)], &random, sizeof(u32));
  }
}

UUID::UUID(i8 const *uuid) {
  if (uuid == nullptr) {
    std::memset(mUUID, 0, sizeof(i8) * sUUIDLength);
    return;
  }
  std::memcpy(mUUID, uuid, sizeof(i8) * sUUIDLength);
}

UUID::UUID(str const &uuid) {
  std::memcpy(mUUID, uuid.c_str(), sizeof(i8) * sUUIDLength);
}

UUID::UUID() { this->generateUUID(); }

UUID::UUID(UUID const &other) {
  std::memcpy(mUUID, other.mUUID, sizeof(i8) * sUUIDLength);
}

UUID::UUID(UUID &&other) {
  std::memcpy(mUUID, other.mUUID, sizeof(i8) * sUUIDLength);
}

i8 const *UUID::raw() const { return mUUID; }

str UUID::toString() const {
  stream ss;
  for (int i = 0; i < sUUIDLength / sizeof(u16); ++i) {
    i16 block = 0;
    std::memcpy(&block, mUUID + i * sizeof(u16), sizeof(u16));
    ss << std::hex << std::setfill('0') << std::setw(4) << block;
    if (i != (sUUIDLength / 2) - 1)
      ss << "-";
  }
  return ss.str();
}

size_t UUID::hash() const { return std::hash<str>{}(this->toString()); }

bool UUID::operator==(UUID const &other) const {
  return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) == 0;
}

bool UUID::operator!=(UUID const &other) const {
  return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) != 0;
}

bool UUID::operator<(UUID const &other) const {
  return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) < 0;
}

bool UUID::operator>(UUID const &other) const {
  return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) > 0;
}

bool UUID::operator<=(UUID const &other) const {
  return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) <= 0;
}

bool UUID::operator>=(UUID const &other) const {
  return std::memcmp(mUUID, other.mUUID, sizeof(i8) * sUUIDLength) >= 0;
}

UUID &UUID::operator=(UUID const &other) {
  this->generateUUID();
  return *this;
}

UUID &UUID::operator=(UUID &&other) {
  std::memcpy(mUUID, other.mUUID, sizeof(i8) * sUUIDLength);
  return *this;
}

UUID::operator size_t() const { return this->hash(); }

UUID::operator str() const { return this->toString(); }

UUID UUID::fromi8(i8 const *uuid) { return UUID(uuid); }

UUID UUID::fromString(str const &uuid) { return UUID(uuid); }

UUID UUID::empty() { return UUID(nullptr); }

UUID UUID::copy(UUID const &uuid) { return UUID::fromString(uuid.toString()); }

} // namespace Terreate

std::ostream &operator<<(std::ostream &stream, Terreate::UUID const &uuid) {
  stream << uuid.toString();
  return stream;
}
