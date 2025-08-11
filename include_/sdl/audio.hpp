#pragma once

#include "../common/enum.hpp"
#include "../common/type.hpp"

namespace Terreate::SDL {
class AudioDevice;

class SDLAudioBuffer {
private:
  union RawAudioData {
    vec<i8> *i8data;
    vec<u8> *u8data;
    vec<i16> *i16data;
    vec<i32> *i32data;
    vec<float> *f32data;
  };

private:
  AudioFormat mFormat;
  RawAudioData mData;

public:
  SDLAudioBuffer(AudioFormat format);
  ~SDLAudioBuffer();

  AudioFormat const &getFormat() const { return mFormat; }
  RawAudioData const &getRawData() const { return mData; }
  vec<i8> const &getI8Data();
  vec<u8> const &getU8Data();
  vec<i16> const &getI16Data();
  vec<i32> const &getI32Data();
  vec<float> const &getF32Data();

  void allocate(u32 size);
  void loadData(vec<i8> const &data);
  void loadData(vec<u8> const &data);
  void loadData(vec<i16> const &data);
  void loadData(vec<i32> const &data);
  void loadData(vec<float> const &data);
};

class AudioStream {
private:
  SDL_AudioStream *mStream = nullptr;
  AudioSpec mSrcSpec;
  AudioSpec mDstSpec;

private:
  friend class AudioDevice;
  AudioStream(SDL_AudioStream *stream);
  SDL_AudioStream *getHandle() const { return mStream; }

public:
  AudioStream(AudioSpec const &srcSpec, AudioSpec const &dstSpec);
  ~AudioStream();

  SDL_AudioDeviceID getBoundDeviceID() const {
    return SDL_GetAudioStreamDevice(mStream);
  }
  AudioSpec getSrcSpec() const { return mSrcSpec; }
  AudioSpec getDstSpec() const { return mDstSpec; }
  i32 getAvailableBytes() const { return SDL_GetAudioStreamAvailable(mStream); }
  float getFrequencyRatio() const {
    return SDL_GetAudioStreamFrequencyRatio(mStream);
  }
  float getGain() const { return SDL_GetAudioStreamGain(mStream); }
  i32 getQueued() const { return SDL_GetAudioStreamQueued(mStream); }
  vec<i32> getInputChannelMap() const;
  vec<i32> getOutputChannelMap() const;
  shared<SDLAudioBuffer> getStreamData(u32 const &size);

  void setFormat(AudioSpec const &srcSpec, AudioSpec const &dstSpec);
  void setFrequencyRatio(float ratio) {
    SDL_SetAudioStreamFrequencyRatio(mStream, ratio);
  }
  void setGain(float gain) { SDL_SetAudioStreamGain(mStream, gain); }
  void setInputChannelMap(vec<i32> const &map) {
    SDL_SetAudioStreamInputChannelMap(mStream, map.data(), map.size());
  }
  void setOutputChannelMap(vec<i32> const &map) {
    SDL_SetAudioStreamOutputChannelMap(mStream, map.data(), map.size());
  }

  bool isBound() const { return SDL_GetAudioStreamDevice(mStream) != 0; }
  bool isPaused() const { return SDL_AudioStreamDevicePaused(mStream); }

  void pause();
  void resume();
  void lock();
  void unlock();
  void pushData(shared<SDLAudioBuffer> const &data);
  void clear();
  void flush();
};

class AudioDevice {
private:
  SDL_AudioDeviceID mID = 0;
  bool mIsRecorder = false;
  AudioSpec mSpec;
  i32 mSamples = 0;

public:
  AudioDevice(SDL_AudioDeviceID id);
  ~AudioDevice();

  i32 getID() const { return mID; }
  AudioSpec getSpec() const { return mSpec; }
  AudioFormat getFormat() const { return (AudioFormat)mSpec.format; }
  i32 const &getFrequency() const { return mSpec.freq; }
  i32 const &getChannels() const { return mSpec.channels; }
  i32 const &getSamples() const { return mSamples; }
  float getGain() const { return SDL_GetAudioDeviceGain(mID); }
  str getName() const { return SDL_GetAudioDeviceName(mID); }
  str getFormatName() const { return SDL_GetAudioFormatName(mSpec.format); }
  vec<i32> getChannelMap() const;
  vec<str> getDrivers() const;

  void setGain(float gain) { SDL_SetAudioDeviceGain(mID, gain); }

  bool isRecorder() const { return mIsRecorder; }
  bool isPaused() const { return SDL_AudioDevicePaused(mID); }
  bool isPhysicalDevice() const { return SDL_IsAudioDevicePhysical(mID); }

  void pause();
  void resume();
  shared<AudioStream> createStream();
  void bindStream(shared<AudioStream> const &stream) {
    SDL_BindAudioStream(mID, stream->getHandle());
  }
  void bindStreams(vec<shared<AudioStream>> const &streams);
  void unbindStream(shared<AudioStream> const &stream) {
    SDL_UnbindAudioStream(stream->getHandle());
  }
  void unbindStreams(vec<shared<AudioStream>> const &streams);

public:
  static vec<AudioDevice> getPlaybackDevices();
  static vec<AudioDevice> getRecordingDevices();
  static str getCurrentDriverName() { return SDL_GetCurrentAudioDriver(); }
  static vec<ubyte> mixAudio(vec<ubyte> const &src, AudioFormat const &format,
                             float gain);
};
} // namespace Terreate::SDL
