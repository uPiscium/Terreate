#include "../../../include/module/sdl/audio.hpp"
#include "../../../include/module/sdl/exception.hpp"

namespace Terreate::SDL {
AudioBuffer::AudioBuffer(AudioFormat format) : mFormat(format) {
  switch (format) {
  case AudioFormat::UNKNOWN:
    throw Exception::AudioError("UNKNOWN format is detected.");
  case AudioFormat::U8:
    mData.u8data = new vec<u8>();
    break;
  case AudioFormat::S8:
    mData.i8data = new vec<i8>();
    break;
  case AudioFormat::S16LE:
  case AudioFormat::S16BE:
    mData.i16data = new vec<i16>();
    break;
  case AudioFormat::S32LE:
  case AudioFormat::S32BE:
    mData.i32data = new vec<i32>();
    break;
  case AudioFormat::F32LE:
  case AudioFormat::F32BE:
    mData.f32data = new vec<float>();
    break;
  }
}

AudioBuffer::~AudioBuffer() {
  switch (mFormat) {
  case AudioFormat::UNKNOWN:
    break;
  case AudioFormat::U8:
    delete mData.u8data;
    break;
  case AudioFormat::S8:
    delete mData.i8data;
    break;
  case AudioFormat::S16LE:
  case AudioFormat::S16BE:
    delete mData.i16data;
    break;
  case AudioFormat::S32LE:
  case AudioFormat::S32BE:
    delete mData.i32data;
    break;
  case AudioFormat::F32LE:
  case AudioFormat::F32BE:
    delete mData.f32data;
    break;
  }
}

vec<i8> const &AudioBuffer::getI8Data() {
  if (mFormat != AudioFormat::S8) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  return *mData.i8data;
}

vec<u8> const &AudioBuffer::getU8Data() {
  if (mFormat != AudioFormat::U8) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  return *mData.u8data;
}

vec<i16> const &AudioBuffer::getI16Data() {
  if (mFormat != AudioFormat::S16LE && mFormat != AudioFormat::S16BE) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  return *mData.i16data;
}

vec<i32> const &AudioBuffer::getI32Data() {
  if (mFormat != AudioFormat::S32LE && mFormat != AudioFormat::S32BE) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  return *mData.i32data;
}

vec<float> const &AudioBuffer::getF32Data() {
  if (mFormat != AudioFormat::F32LE && mFormat != AudioFormat::F32BE) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  return *mData.f32data;
}

void AudioBuffer::allocate(u32 size) {
  switch (mFormat) {
  case AudioFormat::UNKNOWN:
    throw Exception::AudioError("UNKNOWN format is detected.");
  case AudioFormat::U8:
    mData.u8data->resize(size);
    break;
  case AudioFormat::S8:
    mData.i8data->resize(size);
    break;
  case AudioFormat::S16LE:
  case AudioFormat::S16BE:
    mData.i16data->resize(size / sizeof(i16));
    break;
  case AudioFormat::S32LE:
  case AudioFormat::S32BE:
    mData.i32data->resize(size / sizeof(i32));
    break;
  case AudioFormat::F32LE:
  case AudioFormat::F32BE:
    mData.f32data->resize(size / sizeof(float));
    break;
  }
}

void AudioBuffer::loadData(vec<i8> const &data) {
  if (mFormat != AudioFormat::S8) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  *mData.i8data = data;
}

void AudioBuffer::loadData(vec<u8> const &data) {
  if (mFormat != AudioFormat::U8) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  *mData.u8data = data;
}

void AudioBuffer::loadData(vec<i16> const &data) {
  if (mFormat != AudioFormat::S16LE && mFormat != AudioFormat::S16BE) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  *mData.i16data = data;
}

void AudioBuffer::loadData(vec<i32> const &data) {
  if (mFormat != AudioFormat::S32LE && mFormat != AudioFormat::S32BE) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  *mData.i32data = data;
}

void AudioBuffer::loadData(vec<float> const &data) {
  if (mFormat != AudioFormat::F32LE && mFormat != AudioFormat::F32BE) {
    throw Exception::AudioError("Data format does not match buffer format.");
  }
  *mData.f32data = data;
}

AudioStream::AudioStream(SDL_AudioStream *stream) : mStream(stream) {
  if (mStream == nullptr) {
    throw Exception::AudioError("Failed to create audio stream: " +
                                std::string(SDL_GetError()));
  }
  if (!SDL_GetAudioStreamFormat(mStream, &mSrcSpec, &mDstSpec)) {
    throw Exception::AudioError("Failed to get audio stream spec: " +
                                std::string(SDL_GetError()));
  }
}

AudioStream::AudioStream(AudioSpec const &srcSpec, AudioSpec const &dstSpec)
    : mSrcSpec(srcSpec), mDstSpec(dstSpec) {
  mStream = SDL_CreateAudioStream(&mSrcSpec, &mDstSpec);
  if (mStream == nullptr) {
    throw Exception::AudioError("Failed to create audio stream: " +
                                std::string(SDL_GetError()));
  }
}

AudioStream::~AudioStream() {
  if (mStream != nullptr) {
    SDL_DestroyAudioStream(mStream);
  }
}

vec<i32> AudioStream::getInputChannelMap() const {
  i32 numChannels;
  i32 *channelMapPtr = SDL_GetAudioStreamInputChannelMap(mStream, &numChannels);
  if (channelMapPtr == nullptr) {
    throw Exception::AudioError("Failed to get input channel map: " +
                                std::string(SDL_GetError()));
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
    throw Exception::AudioError("Failed to get output channel map: " +
                                std::string(SDL_GetError()));
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
  void *ptr = nullptr;
  switch (format) {
  case AudioFormat::UNKNOWN:
    throw Exception::AudioError("UNKNOWN format is detected.");
  case AudioFormat::U8:
    ptr = (void *)buffer->getU8Data().data();
    break;
  case AudioFormat::S8:
    ptr = (void *)buffer->getI8Data().data();
    break;
  case AudioFormat::S16LE:
  case AudioFormat::S16BE:
    ptr = (void *)buffer->getI16Data().data();
    break;
  case AudioFormat::S32LE:
  case AudioFormat::S32BE:
    ptr = (void *)buffer->getI32Data().data();
    break;
  case AudioFormat::F32LE:
  case AudioFormat::F32BE:
    ptr = (void *)buffer->getF32Data().data();
    break;
  }
  if (SDL_GetAudioStreamData(mStream, ptr, size) < 0) {
    throw Exception::AudioError("Failed to get audio stream data: " +
                                std::string(SDL_GetError()));
  }
  return buffer;
}

void AudioStream::setFormat(AudioSpec const &srcSpec,
                            AudioSpec const &dstSpec) {
  mSrcSpec = srcSpec;
  mDstSpec = dstSpec;
  SDL_SetAudioStreamFormat(mStream, &mSrcSpec, &mDstSpec);
}

void AudioStream::pause() {
  if (!SDL_PauseAudioStreamDevice(mStream)) {
    throw Exception::AudioError("Failed to pause audio stream: " +
                                std::string(SDL_GetError()));
  }
}

void AudioStream::resume() {
  if (!SDL_ResumeAudioStreamDevice(mStream)) {
    throw Exception::AudioError("Failed to resume audio stream: " +
                                std::string(SDL_GetError()));
  }
}

void AudioStream::lock() {
  if (!SDL_LockAudioStream(mStream)) {
    throw Exception::AudioError("Failed to lock audio stream: " +
                                std::string(SDL_GetError()));
  }
}

void AudioStream::unlock() {
  if (!SDL_UnlockAudioStream(mStream)) {
    throw Exception::AudioError("Failed to unlock audio stream: " +
                                std::string(SDL_GetError()));
  }
}

void AudioStream::pushData(shared<AudioBuffer> const &data) {
  bool isRecorder = SDL_IsAudioDevicePlayback(this->getBoundDeviceID());
  AudioFormat format =
      isRecorder ? (AudioFormat)mSrcSpec.format : (AudioFormat)mDstSpec.format;
  if (data->getFormat() != format) {
    throw Exception::AudioError("Data format does not match stream format.");
  }

  void *ptr = nullptr;
  u32 size = 0;
  switch (format) {
  case AudioFormat::UNKNOWN:
    throw Exception::AudioError("UNKNOWN format is detected.");
  case AudioFormat::U8:
    ptr = (void *)data->getU8Data().data();
    size = static_cast<u32>(data->getU8Data().size());
    break;
  case AudioFormat::S8:
    ptr = (void *)data->getI8Data().data();
    size = static_cast<u32>(data->getI8Data().size());
    break;
  case AudioFormat::S16LE:
  case AudioFormat::S16BE:
    ptr = (void *)data->getI16Data().data();
    size = static_cast<u32>(data->getI16Data().size() * sizeof(i16));
    break;
  case AudioFormat::S32LE:
  case AudioFormat::S32BE:
    ptr = (void *)data->getI32Data().data();
    size = static_cast<u32>(data->getI32Data().size() * sizeof(i32));
    break;
  case AudioFormat::F32LE:
  case AudioFormat::F32BE:
    ptr = (void *)data->getF32Data().data();
    size = static_cast<u32>(data->getF32Data().size() * sizeof(float));
    break;
  }
  if (!SDL_PutAudioStreamData(mStream, ptr, size)) {
    throw Exception::AudioError("Failed to get audio stream data: " +
                                std::string(SDL_GetError()));
  }
}

void AudioStream::clear() {
  if (!SDL_ClearAudioStream(mStream)) {
    throw Exception::AudioError("Failed to clear audio buffer: " +
                                std::string(SDL_GetError()));
  }
}

void AudioStream::flush() {
  if (!SDL_FlushAudioStream(mStream)) {
    throw Exception::AudioError("Failed to flush audio buffer: " +
                                std::string(SDL_GetError()));
  }
}

AudioDevice::AudioDevice(SDL_AudioDeviceID id) {
  mID = SDL_OpenAudioDevice(id, nullptr);
  if (mID == 0) {
    throw Exception::AudioError("Failed to open audio device: " +
                                std::string(SDL_GetError()));
  }
  if (!SDL_GetAudioDeviceFormat(mID, &mSpec, &mSamples)) {
    throw Exception::AudioError("Failed to get audio device format: " +
                                std::string(SDL_GetError()));
  }
  mIsRecorder = !SDL_IsAudioDevicePlayback(mID);
}

AudioDevice::~AudioDevice() {
  if (mID != 0) {
    SDL_CloseAudioDevice(mID);
  }
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

void AudioDevice::pause() {
  if (!SDL_PauseAudioDevice(mID)) {
    throw Exception::AudioError("Failed to pause audio device: " +
                                std::string(SDL_GetError()));
  }
}

void AudioDevice::resume() {
  if (!SDL_ResumeAudioDevice(mID)) {
    throw Exception::AudioError("Failed to resume audio device: " +
                                std::string(SDL_GetError()));
  }
}

void AudioDevice::bindStreams(vec<shared<AudioStream>> const &streams) {
  vec<SDL_AudioStream *> ptrs;
  for (auto &stream : streams) {
    ptrs.push_back(stream->getHandle());
  }
  SDL_BindAudioStreams(mID, ptrs.data(), ptrs.size());
}

void AudioDevice::unbindStreams(vec<shared<AudioStream>> const &streams) {
  vec<SDL_AudioStream *> ptrs;
  for (auto &stream : streams) {
    ptrs.push_back(stream->getHandle());
  }
  SDL_UnbindAudioStreams(ptrs.data(), ptrs.size());
}

vec<AudioDevice> AudioDevice::getPlaybackDevices() {
  i32 numDevices;
  SDL_AudioDeviceID *deviceIDs = SDL_GetAudioPlaybackDevices(&numDevices);
  if (deviceIDs == nullptr) {
    throw Exception::AudioError("Failed to get audio playback devices: " +
                                std::string(SDL_GetError()));
  }
  vec<AudioDevice> devices(deviceIDs, deviceIDs + numDevices);
  return devices;
}

vec<AudioDevice> AudioDevice::getRecordingDevices() {
  i32 numDevices;
  SDL_AudioDeviceID *deviceIDs = SDL_GetAudioRecordingDevices(&numDevices);
  if (deviceIDs == nullptr) {
    throw Exception::AudioError("Failed to get audio recording devices: " +
                                std::string(SDL_GetError()));
  }
  vec<AudioDevice> devices(deviceIDs, deviceIDs + numDevices);
  return devices;
}

vec<ubyte> AudioDevice::mixAudio(vec<ubyte> const &src,
                                 AudioFormat const &format, float gain) {
  if (src.empty()) {
    return {};
  }
  i32 srcSize = static_cast<i32>(src.size());
  vec<ubyte> dst(srcSize);
  if (!SDL_MixAudio(dst.data(), src.data(), (SDL_AudioFormat)format, srcSize,
                    gain)) {
    throw Exception::AudioError("Failed to mix audio: " +
                                std::string(SDL_GetError()));
  }
  return dst;
}
} // namespace Terreate::SDL
