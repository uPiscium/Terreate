#ifndef __TERREATEIO_CONTAINERS_AUDIO_HPP__
#define __TERREATEIO_CONTAINERS_AUDIO_HPP__

#include <types.hpp>

namespace Terreate::IO {
using namespace Terreate::Types;

class AudioData {
private:
  Vec<Byte> mData;
  Size mFrequency;

public:
  AudioData() : mData(), mFrequency(0) {}
  AudioData(Vec<Byte> const &data, Size frequency)
      : mData(data), mFrequency(frequency) {}

  Vec<Byte> const &GetData() const { return mData; }
  Size const &GetFrequency() const { return mFrequency; }

  void SetData(Vec<Byte> const &data) { mData = data; }
  void SetFrequency(Size frequency) { mFrequency = frequency; }
};
} // namespace Terreate::IO

#endif // __TERREATEIO_CONTAINERS_AUDIO_HPP__
