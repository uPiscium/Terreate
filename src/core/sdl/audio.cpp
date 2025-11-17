#include "core/sdl/audio.hpp"
#include "core/sdl/exception.hpp"

namespace Terreate::SDL {

AudioBuffer::AudioBuffer(AudioFormat format) : mFormat(format) {
  switch (mFormat) {
  case AudioFormat::UNKNOWN:
    throw AudioException("Cannot create buffer for UNKNOWN format.");
  case AudioFormat::U8:
    mData = vec<u8>();
    break;
  case AudioFormat::S8:
    mData = vec<i8>();
    break;
  case AudioFormat::S16LE:
  case AudioFormat::S16BE:
    mData = vec<i16>();
    break;
  case AudioFormat::S32LE:
  case AudioFormat::S32BE:
    mData = vec<i32>();
    break;
  case AudioFormat::F32LE:
  case AudioFormat::F32BE:
    mData = vec<float>();
    break;
  }
}

AudioBuffer::AudioBuffer(AudioFormat format, vec<i8> const &data)
    : mFormat(format), mData(data) {}

AudioBuffer::AudioBuffer(AudioFormat format, vec<u8> const &data)
    : mFormat(format), mData(data) {}

AudioBuffer::AudioBuffer(AudioFormat format, vec<i16> const &data)
    : mFormat(format), mData(data) {}

AudioBuffer::AudioBuffer(AudioFormat format, vec<i32> const &data)
    : mFormat(format), mData(data) {}

AudioBuffer::AudioBuffer(AudioFormat format, vec<float> const &data)
    : mFormat(format), mData(data) {}

AudioFormat AudioBuffer::getFormat() const { return mFormat; }

RawBuffer AudioBuffer::getRawData() const {
  return std::visit(
      [](auto const &data) -> RawBuffer { return (RawBuffer)data.data(); },
      mData);
}

u32 AudioBuffer::getSizeInBytes() const {
  return std::visit(
      [](auto const &data) -> u32 {
        using T = typename std::decay<decltype(data)>::type::value_type;
        return static_cast<u32>(data.size() * sizeof(T));
      },
      mData);
}

void AudioBuffer::allocate(u32 const &size) {
  std::visit(
      [size](auto &data) {
        using T = typename std::decay<decltype(data)>::type::value_type;
        data.resize(size / sizeof(T));
      },
      mData);
}

void AudioBuffer::load(vec<i8> const &data) { mData = data; }

void AudioBuffer::load(vec<u8> const &data) { mData = data; }

void AudioBuffer::load(vec<i16> const &data) { mData = data; }

void AudioBuffer::load(vec<i32> const &data) { mData = data; }

void AudioBuffer::load(vec<float> const &data) { mData = data; }

AudioStream::AudioStream(SDL_AudioStream *stream) : mStream(stream) {
  if (mStream == nullptr) {
    throw AudioException("Failed to create audio stream: " +
                         str(SDL_GetError()));
  }
  if (!SDL_GetAudioStreamFormat(mStream, &mSrcSpec, &mDstSpec)) {
    throw AudioException("Failed to get audio stream spec: " +
                         str(SDL_GetError()));
  }
}

AudioStream::AudioStream(AudioSpec const &srcSpec, AudioSpec const &dstSpec)
    : mSrcSpec(srcSpec), mDstSpec(dstSpec) {
  mStream = SDL_CreateAudioStream(&mSrcSpec, &mDstSpec);
  if (mStream == nullptr) {
    throw AudioException("Failed to create audio stream: " +
                         str(SDL_GetError()));
  }
}

AudioStream::~AudioStream() {
  if (mStream != nullptr) {
    SDL_DestroyAudioStream(mStream);
  }
}

SDL_AudioDeviceID AudioStream::getBoundDeviceID() const {
  return SDL_GetAudioStreamDevice(mStream);
}

AudioSpec AudioStream::getSrcSpec() const { return mSrcSpec; }

AudioSpec AudioStream::getDstSpec() const { return mDstSpec; }

i32 AudioStream::getAvailableBytes() const {
  return SDL_GetAudioStreamAvailable(mStream);
}

float AudioStream::getFrequencyRatio() const {
  return SDL_GetAudioStreamFrequencyRatio(mStream);
}

float AudioStream::getGain() const { return SDL_GetAudioStreamGain(mStream); }

i32 AudioStream::getQueued() const { return SDL_GetAudioStreamQueued(mStream); }

vec<i32> AudioStream::getInputChannelMap() const {
  i32 numChannels;
  i32 *channelMapPtr = SDL_GetAudioStreamInputChannelMap(mStream, &numChannels);
  if (channelMapPtr == nullptr) {
    throw AudioException("Failed to get input channel map: " +
                         str(SDL_GetError()));
  }
  vec<i32> channelMap(channelMapPtr, channelMapPtr + numChannels);
  SDL_free(channelMapPtr);
  return channelMap;
}

vec<i32> AudioStream::getOutputChannelMap() const {
  i32 numChannels;
  i32 *channelMapPtr =
      SDL_GetAudioStreamOutputChannelMap(mStream, &numChannels);
  if (channelMapPtr == nullptr) {
    throw AudioException("Failed to get output channel map: " +
                         str(SDL_GetError()));
  }
  vec<i32> channelMap(channelMapPtr, channelMapPtr + numChannels);
  SDL_free(channelMapPtr);
  return channelMap;
}

shared<AudioBuffer> AudioStream::getStreamData(u32 const &size) {
  bool isRecorder = SDL_IsAudioDevicePlayback(this->getBoundDeviceID());
  AudioFormat format =
      isRecorder ? (AudioFormat)mSrcSpec.format : (AudioFormat)mDstSpec.format;
  shared<AudioBuffer> buffer = std::make_shared<AudioBuffer>(format);
  buffer->allocate(size);
  void *ptr = buffer->getRawData();
  if (SDL_GetAudioStreamData(mStream, ptr, size) < 0) {
    throw AudioException("Failed to get audio stream data: " +
                         str(SDL_GetError()));
  }
  return buffer;
}

void AudioStream::setFormat(AudioSpec const &srcSpec,
                            AudioSpec const &dstSpec) {
  mSrcSpec = srcSpec;
  mDstSpec = dstSpec;
  SDL_SetAudioStreamFormat(mStream, &mSrcSpec, &mDstSpec);
}

void AudioStream::setFrequencyRatio(float ratio) {
  SDL_SetAudioStreamFrequencyRatio(mStream, ratio);
}

void AudioStream::setGain(float gain) { SDL_SetAudioStreamGain(mStream, gain); }

void AudioStream::setInputChannelMap(vec<i32> const &map) {
  SDL_SetAudioStreamInputChannelMap(mStream, map.data(), map.size());
}

void AudioStream::setOutputChannelMap(vec<i32> const &map) {
  SDL_SetAudioStreamOutputChannelMap(mStream, map.data(), map.size());
}

bool AudioStream::isBound() const {
  return SDL_GetAudioStreamDevice(mStream) != 0;
}

bool AudioStream::isPaused() const {
  return SDL_AudioStreamDevicePaused(mStream);
}

void AudioStream::pause() {
  if (!SDL_PauseAudioStreamDevice(mStream)) {
    throw AudioException("Failed to pause audio stream: " +
                         str(SDL_GetError()));
  }
}

void AudioStream::resume() {
  if (!SDL_ResumeAudioStreamDevice(mStream)) {
    throw AudioException("Failed to resume audio stream: " +
                         str(SDL_GetError()));
  }
}

void AudioStream::lock() {
  if (!SDL_LockAudioStream(mStream)) {
    throw AudioException("Failed to lock audio stream: " + str(SDL_GetError()));
  }
}

void AudioStream::unlock() {
  if (!SDL_UnlockAudioStream(mStream)) {
    throw AudioException("Failed to unlock audio stream: " +
                         str(SDL_GetError()));
  }
}

void AudioStream::pushData(shared<AudioBuffer> const &data) {
  bool isRecorder = SDL_IsAudioDevicePlayback(this->getBoundDeviceID());
  AudioFormat format =
      isRecorder ? (AudioFormat)mSrcSpec.format : (AudioFormat)mDstSpec.format;
  if (data->getFormat() != format) {
    throw AudioException("Data format does not match stream format.");
  }

  void *ptr = data->getRawData();
  u32 size = data->getSizeInBytes();
  if (!SDL_PutAudioStreamData(mStream, ptr, size)) {
    throw AudioException("Failed to get audio stream data: " +
                         str(SDL_GetError()));
  }
}

void AudioStream::clear() {
  if (!SDL_ClearAudioStream(mStream)) {
    throw AudioException("Failed to clear audio buffer: " +
                         str(SDL_GetError()));
  }
}

void AudioStream::flush() {
  if (!SDL_FlushAudioStream(mStream)) {
    throw AudioException("Failed to flush audio buffer: " +
                         str(SDL_GetError()));
  }
}

AudioStream::operator SDL_AudioStream *() const { return mStream; }

AudioDevice::AudioDevice(SDL_AudioDeviceID id) {
  mID = SDL_OpenAudioDevice(id, nullptr);
  if (mID == 0) {
    throw AudioException("Failed to open audio device: " + str(SDL_GetError()));
  }
  if (!SDL_GetAudioDeviceFormat(mID, &mSpec, &mSamples)) {
    throw AudioException("Failed to get audio device format: " +
                         str(SDL_GetError()));
  }
  mIsRecorder = !SDL_IsAudioDevicePlayback(mID);
}

AudioDevice::~AudioDevice() {
  if (mID != 0) {
    SDL_CloseAudioDevice(mID);
  }
}

i32 AudioDevice::getID() const { return mID; }

AudioSpec AudioDevice::getSpec() const { return mSpec; }

AudioFormat AudioDevice::getFormat() const { return (AudioFormat)mSpec.format; }

i32 const &AudioDevice::getFrequency() const { return mSpec.freq; }

i32 const &AudioDevice::getChannels() const { return mSpec.channels; }

i32 const &AudioDevice::getSamples() const { return mSamples; }

float AudioDevice::getGain() const { return SDL_GetAudioDeviceGain(mID); }

str AudioDevice::getName() const { return SDL_GetAudioDeviceName(mID); }

str AudioDevice::getFormatName() const {
  return SDL_GetAudioFormatName(mSpec.format);
}

vec<i32> AudioDevice::getChannelMap() const {
  i32 numChannels;
  i32 *channelMapPtr = SDL_GetAudioDeviceChannelMap(mID, &numChannels);
  vec<i32> channelMap(channelMapPtr, channelMapPtr + numChannels);
  SDL_free(channelMapPtr);
  return channelMap;
}

vec<str> AudioDevice::getDrivers() const {
  i32 numDrivers = SDL_GetNumAudioDrivers();
  vec<str> drivers;
  for (i32 i = 0; i < numDrivers; ++i) {
    drivers.push_back(SDL_GetAudioDriver(i));
  }
  return drivers;
}

void AudioDevice::setGain(float gain) { SDL_SetAudioDeviceGain(mID, gain); }

bool AudioDevice::isRecorder() const { return mIsRecorder; }

bool AudioDevice::isPaused() const { return SDL_AudioDevicePaused(mID); }

bool AudioDevice::isPhysicalDevice() const {
  return SDL_IsAudioDevicePhysical(mID);
}

void AudioDevice::pause() {
  if (!SDL_PauseAudioDevice(mID)) {
    throw AudioException("Failed to pause audio device: " +
                         str(SDL_GetError()));
  }
}

void AudioDevice::resume() {
  if (!SDL_ResumeAudioDevice(mID)) {
    throw AudioException("Failed to resume audio device: " +
                         str(SDL_GetError()));
  }
}

void AudioDevice::bindStream(shared<AudioStream> const &stream) {
  SDL_BindAudioStream(mID, (SDL_AudioStream *)(*stream));
}

void AudioDevice::bindStreams(vec<shared<AudioStream>> const &streams) {
  vec<SDL_AudioStream *> ptrs;
  for (auto &stream : streams) {
    ptrs.push_back((SDL_AudioStream *)(*stream));
  }
  SDL_BindAudioStreams(mID, ptrs.data(), ptrs.size());
}

void AudioDevice::unbindStream(shared<AudioStream> const &stream) {
  SDL_UnbindAudioStream((SDL_AudioStream *)(*stream));
}

void AudioDevice::unbindStreams(vec<shared<AudioStream>> const &streams) {
  vec<SDL_AudioStream *> ptrs;
  for (auto &stream : streams) {
    ptrs.push_back((SDL_AudioStream *)(*stream));
  }
  SDL_UnbindAudioStreams(ptrs.data(), ptrs.size());
}

vec<AudioDevice> AudioDevice::getPlaybackDevices() {
  i32 numDevices;
  SDL_AudioDeviceID *deviceIDs = SDL_GetAudioPlaybackDevices(&numDevices);
  if (deviceIDs == nullptr) {
    throw AudioException("Failed to get audio playback devices: " +
                         str(SDL_GetError()));
  }
  vec<AudioDevice> devices(deviceIDs, deviceIDs + numDevices);
  return devices;
}

vec<AudioDevice> AudioDevice::getRecordingDevices() {
  i32 numDevices;
  SDL_AudioDeviceID *deviceIDs = SDL_GetAudioRecordingDevices(&numDevices);
  if (deviceIDs == nullptr) {
    throw AudioException("Failed to get audio recording devices: " +
                         str(SDL_GetError()));
  }
  vec<AudioDevice> devices(deviceIDs, deviceIDs + numDevices);
  return devices;
}

str AudioDevice::getCurrentDriverName() { return SDL_GetCurrentAudioDriver(); }

vec<ubyte> AudioDevice::mixAudio(vec<ubyte> const &src,
                                 AudioFormat const &format, float gain) {
  if (src.empty()) {
    return {};
  }
  i32 srcSize = static_cast<i32>(src.size());
  vec<ubyte> dst(srcSize);
  if (!SDL_MixAudio(dst.data(), src.data(), (SDL_AudioFormat)format, srcSize,
                    gain)) {
    throw AudioException("Failed to mix audio: " + str(SDL_GetError()));
  }
  return dst;
}

} // namespace Terreate::SDL
