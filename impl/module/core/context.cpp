#include "../../../include/module/core/context.hpp"
#include "../../../include/module/core/exception.hpp"
#include "../../../include/module/sdl.hpp"

namespace Terreate::Core {
Context::Context() {
  SDL::initializeSDL();
  mClock = std::make_shared<Clock>();
  mRegistry = std::make_shared<SDL::SDLObjectRegistry>();
  mEventHandler = std::make_shared<SDL::EventHandler>(mRegistry);
}

Context::~Context() {
  if (mWindow) {
    mWindow->close();
    mWindow.reset();
  }
  mClock.reset();
  mEventHandler.reset();
  mRegistry.reset();
  SDL::terminate();
}

shared<SDL::Window> Context::createWindow(u32 width, u32 height,
                                          str const &title,
                                          bool autoCloseOnEvent) {
  if (mWindow) {
    throw Exception::ContextError("Window already exists. Currently, multiple "
                                  "windows are not supported.");
  }

  shared<SDL::Mouse> mouse;
  if (mRegistry->hasMouse(0)) {
    mouse = mRegistry->getMouse(0);
  } else {
    mouse = std::make_shared<SDL::Mouse>(0);
    mRegistry->registerMouse(0, mouse);
  }
  mWindow = std::make_shared<SDL::Window>(width, height, title, mouse);
  mRegistry->registerWindow(mWindow->getId(), mWindow);

  if (autoCloseOnEvent) {
    mEventHandler->onWindowCloseRequested.subscribe(
        [this](u64, shared<SDL::Window> window) {
          if (window && window->getId() == mWindow->getId()) {
            window->close();
          }
        });
  }

  if (!mGLADInitialized) {
    mWindow->setCurrentContext();
    SDL::initializeGLAD();
    mGLADInitialized = true;
    mWindow->unbind();
  }

  return mWindow;
}

void Context::tick(double fps) {
  if (!this->valid())
    return;

  mEventHandler->poll();
  mClock->tick(fps);
}
} // namespace Terreate::Core
