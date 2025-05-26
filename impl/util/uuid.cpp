#include "../../include/decl/time.hpp"
#include "../../include/util/uuid.hpp"

#include <iomanip>
#include <iostream>

namespace Terreate::Util {
std::mt19937 UUID::sRandomEngine = std::mt19937(std::random_device()());

void UUID::GenerateUUID() {
  time_t epoch = Type::sinceEpoch().count();
  Type::u16 const *epochBlock = reinterpret_cast<Type::u16 const *>(&epoch);
  for (int i = 0; i < sizeof(time_t) / sizeof(Type::u16); ++i) {
    std::memcpy(&mUUID[i * sizeof(Type::u16)],
                &epochBlock[(sizeof(time_t) / sizeof(Type::u16)) - i - 1],
                sizeof(Type::u16));
  }
  Type::u32 randomLength = (sUUIDLength - sizeof(time_t)) / sizeof(Type::u32);
  for (int i = 0; i < randomLength; ++i) {
    Type::u32 random = sRandomEngine();
    std::memcpy(&mUUID[sizeof(time_t) + i * sizeof(Type::u32)], &random,
                sizeof(Type::u32));
  }
}

UUID::UUID(Type::i8 const *uuid) {
  if (uuid == nullptr) {
    std::memset(mUUID, 0, sizeof(Type::i8) * sUUIDLength);
    return;
  }
  std::memcpy(mUUID, uuid, sizeof(Type::i8) * sUUIDLength);
}

Type::str UUID::toString() const {
  Type::Stream ss;
  for (int i = 0; i < sUUIDLength / sizeof(Type::u16); ++i) {
    Type::i16 block = 0;
    std::memcpy(&block, mUUID + i * sizeof(Type::u16), sizeof(Type::u16));
    ss << std::hex << std::setfill('0') << std::setw(4) << block;
    if (i != (sUUIDLength / 2) - 1)
      ss << "-";
  }
  return ss.str();
}

UUID &UUID::operator=(UUID const &other) {
  this->GenerateUUID();
  return *this;
}

UUID &UUID::operator=(UUID &&other) {
  std::memcpy(mUUID, other.mUUID, sizeof(char) * sUUIDLength);
  return *this;
}

} // namespace Terreate::Util

std::ostream &operator<<(std::ostream &stream,
                         Terreate::Util::UUID const &uuid) {
  stream << uuid.toString();
  return stream;
}
