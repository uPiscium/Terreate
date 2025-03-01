#include <audio/source.hpp>
#include <exceptions.hpp>

namespace Terreate::Audio {
using namespace Terreate::Types;

ALSource::~ALSource() {
  alSourcei(mSource, AL_BUFFER, 0);
  if (mSource.Count() <= 1) {
    alDeleteSources(1, mSource);
  }
}

void ALSource::GetProperty(SourceProperty const &property, vec3 &value) const {
  float x, y, z;
  alGetSource3f(mSource, static_cast<ALenum>(property), &x, &y, &z);
  value = vec3(x, y, z);
}

vec3 ALSource::GetPosition() const {
  vec3 position;
  this->GetProperty(SourceProperty::POSITION, position);
  return position;
}
vec3 ALSource::GetVelocity() const {
  vec3 velocity;
  this->GetProperty(SourceProperty::VELOCITY, velocity);
  return velocity;
}

vec3 ALSource::GetDirection() const {
  vec3 direction;
  this->GetProperty(SourceProperty::DIRECTION, direction);
  return direction;
}

Float ALSource::GetPitch() const {
  Float pitch;
  this->GetProperty(SourceProperty::PITCH, pitch);
  return pitch;
}

Float ALSource::GetGain() const {
  Float gain;
  this->GetProperty(SourceProperty::GAIN, gain);
  return gain;
}

Float ALSource::GetVolume() const {
  Float volume;
  this->GetProperty(SourceProperty::GAIN, volume);
  return volume;
}

Float ALSource::GetPlayOffset() const {
  Float offset;
  this->GetProperty(SourceProperty::SEC_OFFSET, offset);
  return offset;
}

Float ALSource::GetSampleOffset() const {
  Float offset;
  this->GetProperty(SourceProperty::SAMPLE_OFFSET, offset);
  return offset;
}

Float ALSource::GetByteOffset() const {
  Float offset;
  this->GetProperty(SourceProperty::BYTE_OFFSET, offset);
  return offset;
}

Float ALSource::GetMaxDistance() const {
  Float distance;
  this->GetProperty(SourceProperty::MAX_DISTANCE, distance);
  return distance;
}

Float ALSource::GetRolloffFactor() const {
  Float factor;
  this->GetProperty(SourceProperty::ROLLOFF_FACTOR, factor);
  return factor;
}

Float ALSource::GetReferenceDistance() const {
  Float distance;
  this->GetProperty(SourceProperty::REFERENCE_DISTANCE, distance);
  return distance;
}

Float ALSource::GetMinGain() const {
  Float gain;
  this->GetProperty(SourceProperty::MIN_GAIN, gain);
  return gain;
}

Float ALSource::GetMaxGain() const {
  Float gain;
  this->GetProperty(SourceProperty::MAX_GAIN, gain);
  return gain;
}

Float ALSource::GetConeOuterGain() const {
  Float gain;
  this->GetProperty(SourceProperty::CONE_OUTER_GAIN, gain);
  return gain;
}

Float ALSource::GetConeInnerAngle() const {
  Float angle;
  this->GetProperty(SourceProperty::CONE_INNER_ANGLE, angle);
  return angle;
}

Float ALSource::GetConeOuterAngle() const {
  Float angle;
  this->GetProperty(SourceProperty::CONE_OUTER_ANGLE, angle);
  return angle;
}

SourceState ALSource::GetState() const {
  Int state;
  this->GetProperty(SourceProperty::SOURCE_STATE, state);
  return static_cast<SourceState>(state);
}

Bool ALSource::GetSourceRelative() const {
  Int relative;
  this->GetProperty(SourceProperty::SOURCE_RELATIVE, relative);
  return static_cast<Bool>(relative);
}

Bool ALSource::GetLooping() const {
  Int loop;
  this->GetProperty(SourceProperty::LOOPING, loop);
  return static_cast<Bool>(loop);
}

void ALSource::SetGain(Float const &gain) const {
  if (gain < 0.0f || gain > 1.0f) {
    throw Exceptions::SourceError("Volume must be between 0.0 and 1.0");
  }
  this->SetProperty(SourceProperty::GAIN, gain);
}

Bool ALSource::IsLooping() const {
  Int loop;
  this->GetProperty(SourceProperty::LOOPING, loop);
  return static_cast<Bool>(loop);
}

Bool ALSource::IsRelative() const {
  Int relative;
  this->GetProperty(SourceProperty::SOURCE_RELATIVE, relative);
  return static_cast<Bool>(relative);
}

Bool ALSource::IsPlaying() const {
  Int state;
  this->GetProperty(SourceProperty::SOURCE_STATE, state);
  return state == static_cast<Int>(SourceState::PLAYING);
}

Bool ALSource::IsPaused() const {
  Int state;
  this->GetProperty(SourceProperty::SOURCE_STATE, state);
  return state == static_cast<Int>(SourceState::PAUSED);
}

Bool ALSource::IsStopped() const {
  Int state;
  this->GetProperty(SourceProperty::SOURCE_STATE, state);
  return state == static_cast<Int>(SourceState::STOPPED);
}

Int AudioStream::GetNumQueuedBuffers() const {
  Int buffers;
  this->GetProperty(SourceProperty::BUFFER_QUEUED, buffers);
  return buffers;
}

Int AudioStream::GetNumProcessedBuffers() const {
  Int buffers;
  this->GetProperty(SourceProperty::BUFFER_PROCESSED, buffers);
  return buffers;
}

} // namespace Terreate::Audio
