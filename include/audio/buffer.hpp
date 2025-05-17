#ifndef __TERREATE_AUDIO_BUFFER_HPP__
#define __TERREATE_AUDIO_BUFFER_HPP__

#include <audio/ALdefs.hpp>
#include <audio/ALhelper.hpp>
#include <core/object.hpp>
#include <core/type.hpp>

namespace Terreate::Audio {
using namespace Terreate::Types;
using namespace Terreate::Audio::ALhelper;

class AudioBuffer {
private:
  Core::Object mBuffer = Core::Object();
  BufferFormat mFormat = BufferFormat::MONO16;
  Vec<i16> mBufferData = Vec<i16>();

  friend class AudioSource;
  friend class AudioStream;

  Core::Object &getBuffer() { return mBuffer; }
  Core::Object const &getBuffer() const { return mBuffer; }

public:
  AudioBuffer(BufferFormat const &format = BufferFormat::MONO16)
      : mFormat(format) {
    alGenBuffers(1, mBuffer);
  }
  ~AudioBuffer();

  Vec<i16> const &getBufferData() const { return mBufferData; }
  void getProperty(BufferProperty const &property, i32 &value) const {
    alGetBufferi(mBuffer, static_cast<ALenum>(property), &value);
  }
  u32 getFrequency() const;
  u32 getBits() const;
  u32 getChannels() const;
  u32 getSize() const;

  void setBuffer(Vec<i16> const &data, u64 const &frequency);
  void setBuffer(i16 const *data, u64 const &size, u64 const &frequency);
};
} // namespace Terreate::Audio

#endif // __TERREATE_AUDIO_BUFFER_HPP__
