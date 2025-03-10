#include <core/uuid.hpp>

#include <iomanip>
#include <iostream>

namespace Terreate::Core {
std::mt19937 UUID::sRandomEngine = std::mt19937(std::random_device()());

void UUID::GenerateUUID() {
  time_t epoch = SinceEpoch().count();
  TCu16 const *epochBlock = reinterpret_cast<TCu16 const *>(&epoch);
  for (int i = 0; i < sizeof(time_t) / sizeof(TCu16); ++i) {
    std::memcpy(&mUUID[i * sizeof(TCu16)],
                &epochBlock[(sizeof(time_t) / sizeof(TCu16)) - i - 1],
                sizeof(TCu16));
  }
  Uint randomLength = (sUUIDLength - sizeof(time_t)) / sizeof(TCu32);
  for (int i = 0; i < randomLength; ++i) {
    TCu32 random = sRandomEngine();
    std::memcpy(&mUUID[sizeof(time_t) + i * sizeof(TCu32)], &random,
                sizeof(TCu32));
  }
}

UUID::UUID(TCi8 const *uuid) {
  if (uuid == nullptr) {
    std::memset(mUUID, 0, sizeof(TCi8) * sUUIDLength);
    return;
  }
  std::memcpy(mUUID, uuid, sizeof(TCi8) * sUUIDLength);
}

Str UUID::ToString() const {
  Stream ss;
  for (int i = 0; i < sUUIDLength / sizeof(TCu16); ++i) {
    TCi16 block = 0;
    std::memcpy(&block, mUUID + i * sizeof(TCu16), sizeof(TCu16));
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

} // namespace Terreate::Core

std::ostream &operator<<(std::ostream &stream,
                         Terreate::Core::UUID const &uuid) {
  stream << uuid.ToString();
  return stream;
}
