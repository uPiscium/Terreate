#include "../../../include/module/common/clock.hpp"

namespace Terreate {
bool Clock::isElapsed(float const &time) {
  float const now = glfwGetTime();
  float const delta = now - mLastTime;
  if (delta >= time) {
    mLastTime = now;
    mDeltaTime = delta;
    mSamplingTime += delta;
    if (++mSampleCount >= mNumSamples) {
      mFPS = mSampleCount / mSamplingTime;
      mSPF = mSamplingTime / mSampleCount;
      mSampleCount = 0;
      mSamplingTime = 0.0f;
    }
    return true;
  }
  return false;
}

void Clock::tick() {
  while (!this->isElapsed(mTickTime)) {
    // Do nothing
  }
}
} // namespace Terreate
