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
  SDL::terminate();
}

shared<SDL::Window> Context::createWindow(u32 width, u32 height,
                                          str const &title) {
  if (mWindow) {
    throw Exception::ContextError("Window already exists. Currently, multiple "
                                  "windows are not supported.");
  }

  mWindow = std::make_shared<SDL::Window>(width, height, title);
  mRegistry->registerWindow(mWindow->getId(), mWindow);

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
