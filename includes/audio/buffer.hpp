#ifndef __TERREATE_AUDIO_BUFFER_HPP__
#define __TERREATE_AUDIO_BUFFER_HPP__

#include <audio/ALdefs.hpp>
#include <audio/ALhelper.hpp>
#include <core/object.hpp>
#include <types.hpp>

namespace Terreate::Audio {
using namespace Terreate::Types;
using namespace Terreate::Audio::ALhelper;

class AudioBuffer {
private:
  Core::Object mBuffer = Core::Object();
  BufferFormat mFormat = BufferFormat::MONO16;
  Vec<Short> mBufferData = Vec<Short>();

  friend class AudioSource;
  friend class AudioStream;

  Core::Object &GetBuffer() { return mBuffer; }
  Core::Object const &GetBuffer() const { return mBuffer; }

public:
  AudioBuffer(BufferFormat const &format = BufferFormat::MONO16)
      : mFormat(format) {
    alGenBuffers(1, mBuffer);
  }
  ~AudioBuffer();

  Vec<Short> const &GetBufferData() const { return mBufferData; }
  void GetProperty(BufferProperty const &property, Int &value) const {
    alGetBufferi(mBuffer, static_cast<ALenum>(property), &value);
  }
  Uint GetFrequency() const;
  Uint GetBits() const;
  Uint GetChannels() const;
  Uint GetSize() const;

  void SetBuffer(Vec<Short> const &data, Size const &frequency);
  void SetBuffer(Short const *data, Ulong const &size, Ulong const &frequency);
};
} // namespace Terreate::Audio

#endif // __TERREATE_AUDIO_BUFFER_HPP__
