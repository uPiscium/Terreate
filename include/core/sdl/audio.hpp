#pragma once

#include "common/type.hpp"

#include "enum.hpp"

namespace Terreate::SDL {

typedef void *RawBuffer;
typedef variant<vec<i8>, vec<u8>, vec<i16>, vec<i32>, vec<float>> AudioData;

class AudioBuffer {
private:
  AudioFormat mFormat;
  AudioData mData;

public:
  AudioBuffer(AudioFormat format);
  AudioBuffer(AudioFormat format, vec<i8> const &data);
  AudioBuffer(AudioFormat format, vec<u8> const &data);
  AudioBuffer(AudioFormat format, vec<i16> const &data);
  AudioBuffer(AudioFormat format, vec<i32> const &data);
  AudioBuffer(AudioFormat format, vec<float> const &data);

  AudioFormat getFormat() const;
  RawBuffer getRawData() const;
  u32 getSizeInBytes() const;

  void allocate(u32 const &size);
  void load(vec<i8> const &data);
  void load(vec<u8> const &data);
  void load(vec<i16> const &data);
  void load(vec<i32> const &data);
  void load(vec<float> const &data);
};

class AudioStream {
private:
  SDL_AudioStream *mStream = nullptr;
  AudioSpec mSrcSpec;
  AudioSpec mDstSpec;

public:
  AudioStream(SDL_AudioStream *stream);
  AudioStream(AudioSpec const &srcSpec, AudioSpec const &dstSpec);
  ~AudioStream();

  SDL_AudioDeviceID getBoundDeviceID() const;
  AudioSpec getSrcSpec() const;
  AudioSpec getDstSpec() const;
  i32 getAvailableBytes() const;
  float getFrequencyRatio() const;
  float getGain() const;
  i32 getQueued() const;
  vec<i32> getInputChannelMap() const;
  vec<i32> getOutputChannelMap() const;
  shared<AudioBuffer> getStreamData(u32 const &size);

  void setFormat(AudioSpec const &srcSpec, AudioSpec const &dstSpec);
  void setFrequencyRatio(float ratio);
  void setGain(float gain);
  void setInputChannelMap(vec<i32> const &map);
  void setOutputChannelMap(vec<i32> const &map);

  bool isBound() const;
  bool isPaused() const;

  void pause();
  void resume();
  void lock();
  void unlock();
  void pushData(shared<AudioBuffer> const &data);
  void clear();
  void flush();

  operator SDL_AudioStream *() const;
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

  i32 getID() const;
  AudioSpec getSpec() const;
  AudioFormat getFormat() const;
  i32 const &getFrequency() const;
  i32 const &getChannels() const;
  i32 const &getSamples() const;
  float getGain() const;
  str getName() const;
  str getFormatName() const;
  vec<i32> getChannelMap() const;
  vec<str> getDrivers() const;

  void setGain(float gain);

  bool isRecorder() const;
  bool isPaused() const;
  bool isPhysicalDevice() const;

  void pause();
  void resume();
  shared<AudioStream> createStream();
  void bindStream(shared<AudioStream> const &stream);
  void bindStreams(vec<shared<AudioStream>> const &streams);
  void unbindStream(shared<AudioStream> const &stream);
  void unbindStreams(vec<shared<AudioStream>> const &streams);

public:
  static vec<AudioDevice> getPlaybackDevices();
  static vec<AudioDevice> getRecordingDevices();
  static str getCurrentDriverName();
  static vec<ubyte> mixAudio(vec<ubyte> const &src, AudioFormat const &format,
                             float gain);
};

} // namespace Terreate::SDL
