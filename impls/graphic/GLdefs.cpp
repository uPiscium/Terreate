#include <exceptions.hpp>
#include <graphic/GLdefs.hpp>
#include <graphic/joystick.hpp>

namespace Terreate::Graphic {

Bool Clock::IsElapsed(Float const &time) {
  Float const now = glfwGetTime();
  Float const delta = now - mLastTime;
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

void Clock::Tick() {
  while (!IsElapsed(mTickTime)) {
    // Do nothing
  }
}

void InitializeGLFW(GLState &state) {
  if (!glfwInit()) {
    throw Exceptions::GraphicsException("Failed to initialize GLFW");
    return;
  }
  state.GLFW_INITIALIZED = true;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

  glfwSetJoystickCallback(JoystickCallback);
}

void InitializeGLAD(GLState &state) {
  if (!gladLoaderLoadGL()) {
    throw Exceptions::GraphicsException("Failed to initialize GLAD");
    return;
  }
  state.GLAD_INITIALIZED = true;
}

void Terminate(GLState &state) {
  if (state.GLAD_INITIALIZED) {
    gladLoaderUnloadGL();
  }
  if (state.GLFW_INITIALIZED) {
    glfwTerminate();
  }
}
} // namespace Terreate::Graphic
