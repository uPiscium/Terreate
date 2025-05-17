#include <audio/source.hpp>
#include <core/exception.hpp>

namespace Terreate::Audio {
using namespace Terreate::Types;

ALSource::ALSource() { alGenSources(1, mSource); }

ALSource::~ALSource() {
  alSourcei(mSource, AL_BUFFER, 0);
  if (mSource.count() <= 1) {
    alDeleteSources(1, mSource);
  }
}

void ALSource::getProperty(SourceProperty const &property, i32 &value) const {
  alGetSourcei(mSource, static_cast<ALenum>(property), &value);
}

void ALSource::getProperty(SourceProperty const &property, float &value) const {
  alGetSourcef(mSource, static_cast<ALenum>(property), &value);
}

void ALSource::getProperty(SourceProperty const &property, vec3 &value) const {
  float x, y, z;
  alGetSource3f(mSource, static_cast<ALenum>(property), &x, &y, &z);
  value = vec3(x, y, z);
}

vec3 ALSource::getPosition() const {
  vec3 position;
  this->getProperty(SourceProperty::POSITION, position);
  return position;
}
vec3 ALSource::getVelocity() const {
  vec3 velocity;
  this->getProperty(SourceProperty::VELOCITY, velocity);
  return velocity;
}

vec3 ALSource::getDirection() const {
  vec3 direction;
  this->getProperty(SourceProperty::DIRECTION, direction);
  return direction;
}

float ALSource::getPitch() const {
  float pitch;
  this->getProperty(SourceProperty::PITCH, pitch);
  return pitch;
}

float ALSource::getGain() const {
  float gain;
  this->getProperty(SourceProperty::GAIN, gain);
  return gain;
}

float ALSource::getVolume() const {
  float volume;
  this->getProperty(SourceProperty::GAIN, volume);
  return volume;
}

float ALSource::getPlayOffset() const {
  float offset;
  this->getProperty(SourceProperty::SEC_OFFSET, offset);
  return offset;
}

float ALSource::getSampleOffset() const {
  float offset;
  this->getProperty(SourceProperty::SAMPLE_OFFSET, offset);
  return offset;
}

float ALSource::getByteOffset() const {
  float offset;
  this->getProperty(SourceProperty::BYTE_OFFSET, offset);
  return offset;
}

float ALSource::getMaxDistance() const {
  float distance;
  this->getProperty(SourceProperty::MAX_DISTANCE, distance);
  return distance;
}

float ALSource::getRolloffFactor() const {
  float factor;
  this->getProperty(SourceProperty::ROLLOFF_FACTOR, factor);
  return factor;
}

float ALSource::getReferenceDistance() const {
  float distance;
  this->getProperty(SourceProperty::REFERENCE_DISTANCE, distance);
  return distance;
}

float ALSource::getMinGain() const {
  float gain;
  this->getProperty(SourceProperty::MIN_GAIN, gain);
  return gain;
}

float ALSource::getMaxGain() const {
  float gain;
  this->getProperty(SourceProperty::MAX_GAIN, gain);
  return gain;
}

float ALSource::getConeOuterGain() const {
  float gain;
  this->getProperty(SourceProperty::CONE_OUTER_GAIN, gain);
  return gain;
}

float ALSource::getConeInnerAngle() const {
  float angle;
  this->getProperty(SourceProperty::CONE_INNER_ANGLE, angle);
  return angle;
}

float ALSource::getConeOuterAngle() const {
  float angle;
  this->getProperty(SourceProperty::CONE_OUTER_ANGLE, angle);
  return angle;
}

SourceState ALSource::getState() const {
  i32 state;
  this->getProperty(SourceProperty::SOURCE_STATE, state);
  return static_cast<SourceState>(state);
}

bool ALSource::getSourceRelative() const {
  i32 relative;
  this->getProperty(SourceProperty::SOURCE_RELATIVE, relative);
  return static_cast<bool>(relative);
}

bool ALSource::getLooping() const {
  i32 loop;
  this->getProperty(SourceProperty::LOOPING, loop);
  return static_cast<bool>(loop);
}

void ALSource::setProperty(SourceProperty const &property,
                           i32 const &value) const {
  alSourcei(mSource, static_cast<ALenum>(property), value);
}
void ALSource::setProperty(SourceProperty const &property,
                           float const &value) const {
  alSourcef(mSource, static_cast<ALenum>(property), value);
}
void ALSource::setProperty(SourceProperty const &property,
                           vec3 const &value) const {
  alSource3f(mSource, static_cast<ALenum>(property), value.x, value.y, value.z);
}
void ALSource::setPosition(float const &x, float const &y,
                           float const &z) const {
  this->setProperty(SourceProperty::POSITION, vec3(x, y, z));
}
void ALSource::setPosition(vec3 const &position) const {
  this->setProperty(SourceProperty::POSITION, position);
}
void ALSource::setVelocity(float const &x, float const &y,
                           float const &z) const {
  this->setProperty(SourceProperty::VELOCITY, vec3(x, y, z));
}
void ALSource::setVelocity(vec3 const &velocity) const {
  this->setProperty(SourceProperty::VELOCITY, velocity);
}
void ALSource::setDirection(float const &x, float const &y,
                            float const &z) const {
  this->setProperty(SourceProperty::DIRECTION, vec3(x, y, z));
}
void ALSource::setDirection(vec3 const &direction) const {
  this->setProperty(SourceProperty::DIRECTION, direction);
}
void ALSource::setPitch(float const &pitch) const {
  this->setProperty(SourceProperty::PITCH, pitch);
}

void ALSource::setGain(float const &gain) const {
  if (gain < 0.0f || gain > 1.0f) {
    throw Exceptions::SourceError("Volume must be between 0.0 and 1.0");
  }
  this->setProperty(SourceProperty::GAIN, gain);
}

void ALSource::setVolume(float const &volume) const { this->setGain(volume); }

void ALSource::setPlayOffset(float const &seconds) const {
  this->setProperty(SourceProperty::SEC_OFFSET, seconds);
}

void ALSource::setSampleOffset(float const &samples) const {
  this->setProperty(SourceProperty::SAMPLE_OFFSET, samples);
}

void ALSource::setByteOffset(float const &bytes) const {
  this->setProperty(SourceProperty::BYTE_OFFSET, bytes);
}

void ALSource::setMaxDistance(float const &distance) const {
  this->setProperty(SourceProperty::MAX_DISTANCE, distance);
}

void ALSource::setRolloffFactor(float const &factor) const {
  this->setProperty(SourceProperty::ROLLOFF_FACTOR, factor);
}

void ALSource::setReferenceDistance(float const &distance) const {
  this->setProperty(SourceProperty::REFERENCE_DISTANCE, distance);
}

void ALSource::setMinGain(float const &gain) const {
  this->setProperty(SourceProperty::MIN_GAIN, gain);
}

void ALSource::setMaxGain(float const &gain) const {
  this->setProperty(SourceProperty::MAX_GAIN, gain);
}

void ALSource::setConeOuterGain(float const &gain) const {
  this->setProperty(SourceProperty::CONE_OUTER_GAIN, gain);
}

void ALSource::setConeInnerAngle(float const &angle) const {
  this->setProperty(SourceProperty::CONE_INNER_ANGLE, angle);
}

void ALSource::setConeOuterAngle(float const &angle) const {
  this->setProperty(SourceProperty::CONE_OUTER_ANGLE, angle);
}

void ALSource::setSourceRelative(bool const &relative) const {
  this->setProperty(SourceProperty::SOURCE_RELATIVE, relative);
}

void ALSource::setLooping(bool const &loop) const {
  this->setProperty(SourceProperty::LOOPING, loop);
}

bool ALSource::isLooping() const {
  i32 loop;
  this->getProperty(SourceProperty::LOOPING, loop);
  return static_cast<bool>(loop);
}

bool ALSource::isRelative() const {
  i32 relative;
  this->getProperty(SourceProperty::SOURCE_RELATIVE, relative);
  return static_cast<bool>(relative);
}

bool ALSource::isPlaying() const {
  i32 state;
  this->getProperty(SourceProperty::SOURCE_STATE, state);
  return state == static_cast<i32>(SourceState::PLAYING);
}

bool ALSource::isPaused() const {
  i32 state;
  this->getProperty(SourceProperty::SOURCE_STATE, state);
  return state == static_cast<i32>(SourceState::PAUSED);
}

bool ALSource::isStopped() const {
  i32 state;
  this->getProperty(SourceProperty::SOURCE_STATE, state);
  return state == static_cast<i32>(SourceState::STOPPED);
}

void ALSource::play() { alSourcePlay(mSource); }

void ALSource::stop() { alSourceStop(mSource); }

void ALSource::pause() { alSourcePause(mSource); }

i32 AudioStream::getNumQueuedBuffers() const {
  i32 buffers;
  this->getProperty(SourceProperty::BUFFER_QUEUED, buffers);
  return buffers;
}

i32 AudioStream::getNumProcessedBuffers() const {
  i32 buffers;
  this->getProperty(SourceProperty::BUFFER_PROCESSED, buffers);
  return buffers;
}

} // namespace Terreate::Audio
