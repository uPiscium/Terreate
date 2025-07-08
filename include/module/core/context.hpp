#pragma once

#include "../common/clock.hpp"
#include "../sdl/event.hpp"
#include "../sdl/registry.hpp"
#include "../sdl/window.hpp"

namespace Terreate::Core {
class Context {
private:
  bool mGLADInitialized = false;
  shared<Clock> mClock;
  shared<SDL::Window> mWindow;
  shared<SDL::SDLObjectRegistry> mRegistry =
      std::make_shared<SDL::SDLObjectRegistry>();
  shared<SDL::EventHandler> mEventHandler;

public:
  Context();
  ~Context();

  double getTime() const { return mClock->getCurrentRuntime(); }
  shared<Clock> getClock() const { return mClock; }
  shared<SDL::EventHandler> getEventHandler() const { return mEventHandler; }

  shared<SDL::Window> createWindow(u32 width, u32 height, str const &title,
                                   bool autoCloseOnEvent = true);

  bool valid() const { return mWindow && !mWindow->isClosed(); }
  void tick(double fps);

  operator bool() const { return mWindow && !mWindow->isClosed(); }
};
} // namespace Terreate::Core
