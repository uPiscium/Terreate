// #include <iomanip>
// #include <iostream>

// #include <time.hpp>
// #include <uuid.hpp>

// namespace Terreate::Core {
// std::mt19937 UUID::sRandomEngine = std::mt19937(std::random_device()());

// void UUID::GenerateUUID() {
//   time_t epoch = sinceEpoch().count();
//   u16 const *epochBlock = reinterpret_cast<u16 const *>(&epoch);
//   for (int i = 0; i < sizeof(time_t) / sizeof(u16); ++i) {
//     std::memcpy(&mUUID[i * sizeof(u16)],
//                 &epochBlock[(sizeof(time_t) / sizeof(u16)) - i - 1],
//                 sizeof(u16));
//   }
//   u32 randomLength = (sUUIDLength - sizeof(time_t)) / sizeof(u32);
//   for (int i = 0; i < randomLength; ++i) {
//     u32 random = sRandomEngine();
//     std::memcpy(&mUUID[sizeof(time_t) + i * sizeof(u32)], &random,
//     sizeof(u32));
//   }
// }

// UUID::UUID(i8 const *uuid) {
//   if (uuid == nullptr) {
//     std::memset(mUUID, 0, sizeof(i8) * sUUIDLength);
//     return;
//   }
//   std::memcpy(mUUID, uuid, sizeof(i8) * sUUIDLength);
// }

// str UUID::toString() const {
//   Stream ss;
//   for (int i = 0; i < sUUIDLength / sizeof(u16); ++i) {
//     i16 block = 0;
//     std::memcpy(&block, mUUID + i * sizeof(u16), sizeof(u16));
//     ss << std::hex << std::setfill('0') << std::setw(4) << block;
//     if (i != (sUUIDLength / 2) - 1)
//       ss << "-";
//   }
//   return ss.str();
// }

// UUID &UUID::operator=(UUID const &other) {
//   this->GenerateUUID();
//   return *this;
// }

// UUID &UUID::operator=(UUID &&other) {
//   std::memcpy(mUUID, other.mUUID, sizeof(char) * sUUIDLength);
//   return *this;
// }

// } // namespace Terreate::Core

// std::ostream &operator<<(std::ostream &stream,
//                          Terreate::Core::UUID const &uuid) {
//   stream << uuid.toString();
//   return stream;
// }
