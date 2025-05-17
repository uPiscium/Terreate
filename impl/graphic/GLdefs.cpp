#include <core/exception.hpp>
#include <graphic/GLdefs.hpp>
#include <graphic/joystick.hpp>

namespace Terreate::Graphic {

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
  while (!isElapsed(mTickTime)) {
    // Do nothing
  }
}

void initializeGLFW() {
  if (!glfwInit()) {
    throw Exceptions::GraphicsException("Failed to initialize GLFW");
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

  glfwSetJoystickCallback(joystickCallback);
}

void initializeGLAD() {
  if (!gladLoaderLoadGL()) {
    throw Exceptions::GraphicsException("Failed to initialize GLAD");
    return;
  }
}

void terminate() {
  gladLoaderUnloadGL();
  glfwTerminate();
}
} // namespace Terreate::Graphic
