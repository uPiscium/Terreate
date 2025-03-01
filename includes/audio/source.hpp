#ifndef __TERREATEAUDIO_SOURCE_HPP__
#define __TERREATEAUDIO_SOURCE_HPP__

#include <audio/ALdefs.hpp>
#include <audio/ALhelper.hpp>
#include <audio/buffer.hpp>
#include <core/math.hpp>
#include <core/object.hpp>
#include <types.hpp>

namespace Terreate::Audio {
using namespace Terreate::Types;
using namespace Terreate::Math;
using namespace Terreate::Audio::ALhelper;

class ALSource {
protected:
  Core::Object mSource = Core::Object();

public:
  ALSource() { alGenSources(1, mSource); }
  virtual ~ALSource();

  void GetProperty(SourceProperty const &property, Int &value) const {
    alGetSourcei(mSource, static_cast<ALenum>(property), &value);
  }
  void GetProperty(SourceProperty const &property, Float &value) const {
    alGetSourcef(mSource, static_cast<ALenum>(property), &value);
  }
  void GetProperty(SourceProperty const &property, vec3 &value) const;
  vec3 GetPosition() const;
  vec3 GetVelocity() const;
  vec3 GetDirection() const;
  Float GetPitch() const;
  Float GetGain() const;
  Float GetVolume() const;
  Float GetPlayOffset() const;
  Float GetSampleOffset() const;
  Float GetByteOffset() const;
  Float GetMaxDistance() const;
  Float GetRolloffFactor() const;
  Float GetReferenceDistance() const;
  Float GetMinGain() const;
  Float GetMaxGain() const;
  Float GetConeOuterGain() const;
  Float GetConeInnerAngle() const;
  Float GetConeOuterAngle() const;
  SourceState GetState() const;
  Bool GetSourceRelative() const;
  Bool GetLooping() const;

  void SetProperty(SourceProperty const &property, Int const &value) const {
    alSourcei(mSource, static_cast<ALenum>(property), value);
  }
  void SetProperty(SourceProperty const &property, Float const &value) const {
    alSourcef(mSource, static_cast<ALenum>(property), value);
  }
  void SetProperty(SourceProperty const &property, vec3 const &value) const {
    alSource3f(mSource, static_cast<ALenum>(property), value.x, value.y,
               value.z);
  }
  void SetPosition(Float const &x, Float const &y, Float const &z) const {
    this->SetProperty(SourceProperty::POSITION, vec3(x, y, z));
  }
  void SetPosition(vec3 const &position) const {
    this->SetProperty(SourceProperty::POSITION, position);
  }
  void SetVelocity(Float const &x, Float const &y, Float const &z) const {
    this->SetProperty(SourceProperty::VELOCITY, vec3(x, y, z));
  }
  void SetVelocity(vec3 const &velocity) const {
    this->SetProperty(SourceProperty::VELOCITY, velocity);
  }
  void SetDirection(Float const &x, Float const &y, Float const &z) const {
    this->SetProperty(SourceProperty::DIRECTION, vec3(x, y, z));
  }
  void SetDirection(vec3 const &direction) const {
    this->SetProperty(SourceProperty::DIRECTION, direction);
  }
  void SetPitch(Float const &pitch) const {
    this->SetProperty(SourceProperty::PITCH, pitch);
  }
  void SetGain(Float const &gain) const;
  void SetVolume(Float const &volume) const { this->SetGain(volume); }
  void SetPlayOffset(Float const &seconds) const {
    this->SetProperty(SourceProperty::SEC_OFFSET, seconds);
  }
  void SetSampleOffset(Float const &samples) const {
    this->SetProperty(SourceProperty::SAMPLE_OFFSET, samples);
  }
  void SetByteOffset(Float const &bytes) const {
    this->SetProperty(SourceProperty::BYTE_OFFSET, bytes);
  }
  void SetMaxDistance(Float const &distance) const {
    this->SetProperty(SourceProperty::MAX_DISTANCE, distance);
  }
  void SetRolloffFactor(Float const &factor) const {
    this->SetProperty(SourceProperty::ROLLOFF_FACTOR, factor);
  }
  void SetReferenceDistance(Float const &distance) const {
    this->SetProperty(SourceProperty::REFERENCE_DISTANCE, distance);
  }
  void SetMinGain(Float const &gain) const {
    this->SetProperty(SourceProperty::MIN_GAIN, gain);
  }
  void SetMaxGain(Float const &gain) const {
    this->SetProperty(SourceProperty::MAX_GAIN, gain);
  }
  void SetConeOuterGain(Float const &gain) const {
    this->SetProperty(SourceProperty::CONE_OUTER_GAIN, gain);
  }
  void SetConeInnerAngle(Float const &angle) const {
    this->SetProperty(SourceProperty::CONE_INNER_ANGLE, angle);
  }
  void SetConeOuterAngle(Float const &angle) const {
    this->SetProperty(SourceProperty::CONE_OUTER_ANGLE, angle);
  }
  void SetSourceRelative(Bool const &relative) const {
    this->SetProperty(SourceProperty::SOURCE_RELATIVE, relative);
  }
  void SetLooping(Bool const &loop) const {
    this->SetProperty(SourceProperty::LOOPING, loop);
  }

  Bool IsLooping() const;
  Bool IsRelative() const;
  Bool IsPlaying() const;
  Bool IsPaused() const;
  Bool IsStopped() const;

  virtual void Play() { alSourcePlay(mSource); }
  virtual void Stop() { alSourceStop(mSource); }
  virtual void Pause() { alSourcePause(mSource); }
};

class AudioSource : public ALSource {
public:
  AudioSource() = default;
  virtual ~AudioSource() = default;

  void AttachBuffer(AudioBuffer const &buffer) const {
    alSourcei(mSource, AL_BUFFER, buffer.GetBuffer());
  }
  void DetachBuffer() const { alSourcei(mSource, AL_BUFFER, 0); }
};

class AudioStream : public ALSource {
public:
  AudioStream() = default;
  virtual ~AudioStream() = default;

  Int GetNumQueuedBuffers() const;
  Int GetNumProcessedBuffers() const;

  void QueueBuffer(AudioBuffer const &buffer) const {
    alSourceQueueBuffers(mSource, 1, buffer.GetBuffer());
  }
  void UnqueueBuffer(AudioBuffer &buffer) const {
    alSourceUnqueueBuffers(mSource, 1, buffer.GetBuffer());
  }
};
} // namespace Terreate::Audio

#endif // __TERREATEAUDIO_SOURCE_HPP__
