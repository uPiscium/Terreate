#pragma once

#include "../common/clock.hpp"

#include "../opengl/window.hpp"

namespace Terreate::Core {
class Context {
private:
  bool mGLADInitialized = false;
  shared<Clock> mClock;
  shared<OpenGL::Window> mWindow;

public:
  Context();
  ~Context();

  double getTime() const { return mClock->getCurrentRuntime(); }
  shared<Clock> getClock() const { return mClock; }

  shared<OpenGL::Window> createWindow(
      u32 width, u32 height, str const &title,
      OpenGL::WindowSettings const &settings = OpenGL::WindowSettings());

  bool valid() const { return mWindow && !mWindow->isClosed(); }
  void tick(double fps);

  operator bool() const { return mWindow && !mWindow->isClosed(); }
};
} // namespace Terreate::Core
