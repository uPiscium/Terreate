#include <audio/context.hpp>
#include <exceptions.hpp>

namespace Terreate::Audio {
AudioContext::AudioContext() {
  mDevice = alcOpenDevice(NULL);
  if (!mDevice) {
    throw Exceptions::OpenALException("Failed to open device");
  }

  mContext = alcCreateContext(mDevice, nullptr);
  if (!mContext) {
    alcCloseDevice(mDevice);
    throw Exceptions::OpenALException("Failed to create context");
  }

  this->SetCurrent();
}

AudioContext::~AudioContext() {
  alcMakeContextCurrent(nullptr);
  if (mContext) {
    alcDestroyContext(mContext);
  }
  if (mDevice) {
    alcCloseDevice(mDevice);
  }
}

} // namespace Terreate::Audio
