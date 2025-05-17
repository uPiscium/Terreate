#include <audio/buffer.hpp>

namespace Terreate::Audio {
AudioBuffer::~AudioBuffer() {
  if (mBuffer.count() <= 1) {
    alDeleteBuffers(1, mBuffer);
  }
}

u32 AudioBuffer::getFrequency() const {
  i32 frequency;
  this->getProperty(BufferProperty::FREQUENCY, frequency);
  return frequency;
}

u32 AudioBuffer::getBits() const {
  i32 bits;
  this->getProperty(BufferProperty::BITS, bits);
  return bits;
}

u32 AudioBuffer::getChannels() const {
  i32 channels;
  this->getProperty(BufferProperty::CHANNELS, channels);
  return channels;
}

u32 AudioBuffer::getSize() const {
  i32 size;
  this->getProperty(BufferProperty::SIZE, size);
  return size;
}

void AudioBuffer::setBuffer(Vec<i16> const &data, u64 const &frequency) {
  mBufferData = data;
  alBufferData(mBuffer, static_cast<ALenum>(mFormat), mBufferData.data(),
               mBufferData.size() * sizeof(i16), frequency);
}

void AudioBuffer::setBuffer(i16 const *data, u64 const &size,
                            u64 const &frequency) {
  mBufferData = Vec<i16>(data, data + size);
  alBufferData(mBuffer, static_cast<ALenum>(mFormat), mBufferData.data(),
               mBufferData.size() * sizeof(i16), frequency);
}

} // namespace Terreate::Audio
