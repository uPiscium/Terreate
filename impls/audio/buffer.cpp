#include <audio/buffer.hpp>

namespace Terreate::Audio {
AudioBuffer::~AudioBuffer() {
  if (mBuffer.Count() <= 1) {
    alDeleteBuffers(1, mBuffer);
  }
}

Uint AudioBuffer::GetFrequency() const {
  Int frequency;
  this->GetProperty(BufferProperty::FREQUENCY, frequency);
  return frequency;
}

Uint AudioBuffer::GetBits() const {
  Int bits;
  this->GetProperty(BufferProperty::BITS, bits);
  return bits;
}

Uint AudioBuffer::GetChannels() const {
  Int channels;
  this->GetProperty(BufferProperty::CHANNELS, channels);
  return channels;
}

Uint AudioBuffer::GetSize() const {
  Int size;
  this->GetProperty(BufferProperty::SIZE, size);
  return size;
}

void AudioBuffer::SetBuffer(Vec<Short> const &data, Size const &frequency) {
  mBufferData = data;
  alBufferData(mBuffer, static_cast<ALenum>(mFormat), mBufferData.data(),
               mBufferData.size() * sizeof(Short), frequency);
}

void AudioBuffer::SetBuffer(Short const *data, Ulong const &size,
                            Ulong const &frequency) {
  mBufferData = Vec<Short>(data, data + size);
  alBufferData(mBuffer, static_cast<ALenum>(mFormat), mBufferData.data(),
               mBufferData.size() * sizeof(Short), frequency);
}

} // namespace Terreate::Audio
