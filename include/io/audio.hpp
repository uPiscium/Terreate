#ifndef __TERREATEIO_CONTAINERS_AUDIO_HPP__
#define __TERREATEIO_CONTAINERS_AUDIO_HPP__

#include <core/type.hpp>

namespace Terreate::IO {
using namespace Terreate::Types;

class AudioData {
private:
  Vec<Byte> mData;
  u64 mFrequency;

public:
  AudioData() : mData(), mFrequency(0) {}
  AudioData(Vec<Byte> const &data, u64 frequency)
      : mData(data), mFrequency(frequency) {}

  Vec<Byte> const &GetData() const { return mData; }
  u64 const &GetFrequency() const { return mFrequency; }

  void SetData(Vec<Byte> const &data) { mData = data; }
  void SetFrequency(u64 frequency) { mFrequency = frequency; }
};
} // namespace Terreate::IO

#endif // __TERREATEIO_CONTAINERS_AUDIO_HPP__
