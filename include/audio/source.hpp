#ifndef __TERREATEAUDIO_SOURCE_HPP__
#define __TERREATEAUDIO_SOURCE_HPP__

#include <audio/ALdefs.hpp>
#include <audio/ALhelper.hpp>
#include <audio/buffer.hpp>
#include <core/math.hpp>
#include <core/object.hpp>
#include <core/type.hpp>

namespace Terreate::Audio {
using namespace Terreate::Types;
using namespace Terreate::Math;
using namespace Terreate::Audio::ALhelper;

class ALSource {
protected:
  Core::Object mSource = Core::Object();

public:
  ALSource();
  virtual ~ALSource();

  void getProperty(SourceProperty const &property, i32 &value) const;
  void getProperty(SourceProperty const &property, float &value) const;
  void getProperty(SourceProperty const &property, vec3 &value) const;
  vec3 getPosition() const;
  vec3 getVelocity() const;
  vec3 getDirection() const;
  float getPitch() const;
  float getGain() const;
  float getVolume() const;
  float getPlayOffset() const;
  float getSampleOffset() const;
  float getByteOffset() const;
  float getMaxDistance() const;
  float getRolloffFactor() const;
  float getReferenceDistance() const;
  float getMinGain() const;
  float getMaxGain() const;
  float getConeOuterGain() const;
  float getConeInnerAngle() const;
  float getConeOuterAngle() const;
  SourceState getState() const;
  bool getSourceRelative() const;
  bool getLooping() const;

  void setProperty(SourceProperty const &property, i32 const &value) const;
  void setProperty(SourceProperty const &property, float const &value) const;
  void setProperty(SourceProperty const &property, vec3 const &value) const;
  void setPosition(float const &x, float const &y, float const &z) const;
  void setPosition(vec3 const &position) const;
  void setVelocity(float const &x, float const &y, float const &z) const;
  void setVelocity(vec3 const &velocity) const;
  void setDirection(float const &x, float const &y, float const &z) const;
  void setDirection(vec3 const &direction) const;
  void setPitch(float const &pitch) const;
  void setGain(float const &gain) const;
  void setVolume(float const &volume) const;
  void setPlayOffset(float const &seconds) const;
  void setSampleOffset(float const &samples) const;
  void setByteOffset(float const &bytes) const;
  void setMaxDistance(float const &distance) const;
  void setRolloffFactor(float const &factor) const;
  void setReferenceDistance(float const &distance) const;
  void setMinGain(float const &gain) const;
  void setMaxGain(float const &gain) const;
  void setConeOuterGain(float const &gain) const;
  void setConeInnerAngle(float const &angle) const;
  void setConeOuterAngle(float const &angle) const;
  void setSourceRelative(bool const &relative) const;
  void setLooping(bool const &loop) const;

  bool isLooping() const;
  bool isRelative() const;
  bool isPlaying() const;
  bool isPaused() const;
  bool isStopped() const;

  virtual void play();
  virtual void stop();
  virtual void pause();
};

class AudioSource : public ALSource {
public:
  AudioSource() = default;
  virtual ~AudioSource() = default;

  void attachBuffer(AudioBuffer const &buffer) const {
    alSourcei(mSource, AL_BUFFER, buffer.getBuffer());
  }
  void detachBuffer() const { alSourcei(mSource, AL_BUFFER, 0); }
};

class AudioStream : public ALSource {
public:
  AudioStream() = default;
  virtual ~AudioStream() = default;

  i32 getNumQueuedBuffers() const;
  i32 getNumProcessedBuffers() const;

  void queueBuffer(AudioBuffer const &buffer) const {
    alSourceQueueBuffers(mSource, 1, buffer.getBuffer());
  }
  void unqueueBuffer(AudioBuffer &buffer) const {
    alSourceUnqueueBuffers(mSource, 1, buffer.getBuffer());
  }
};
} // namespace Terreate::Audio

#endif // __TERREATEAUDIO_SOURCE_HPP__
