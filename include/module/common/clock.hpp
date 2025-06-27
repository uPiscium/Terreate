#pragma once

#include "type.hpp"

namespace Terreate {
class Clock {
private:
  float mLastTime = 0.0f;
  float mDeltaTime = 0.0f;
  float mSamplingTime = 0.0f;
  u32 mNumSamples = 0u;
  u32 mSampleCount = 0u;
  float mTickTime = 0.0f;
  float mFPS = 0.0f;
  float mSPF = 0.0f;

private:
  bool isElapsed(float const &time);

public:
  Clock(float const &fps = 60, u32 const &samples = 10)
      : mTickTime(1.0f / fps), mNumSamples(samples) {}
  ~Clock() {}

  float getFPS() const { return mFPS; }
  float getSPF() const { return mSPF; }

  void tick();

public:
  static double getCurrentRuntime() { return glfwGetTime(); }
};
} // namespace Terreate
