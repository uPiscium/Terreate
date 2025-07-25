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
  float mFPS = 0.0f;
  float mSPF = 0.0f;

private:
  bool isElapsed(float const &time);

public:
  Clock(u32 const &samples = 10) : mNumSamples(samples) {}
  ~Clock() {}

  float const &getFPS() const { return mFPS; }
  float const &getSPF() const { return mSPF; }
  float const &getDeltaTime() const { return mDeltaTime; }

  void tick(float fps);

public:
  static double getCurrentRuntime() { return SDL_GetTicks() / 1000.0f; }
};
} // namespace Terreate
