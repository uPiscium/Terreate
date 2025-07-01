#include "../../../include/module/core/context.hpp"
#include "../../../include/module/core/exception.hpp"
#include "../../../include/module/opengl.hpp"

namespace Terreate::Core {
Context::Context() {
  OpenGL::initializeGLFW();
  mClock = std::make_shared<Clock>();
}

Context::~Context() {
  if (mWindow) {
    mWindow->close();
    mWindow.reset();
  }
  mClock.reset();
  OpenGL::terminate();
}

shared<OpenGL::Window>
Context::createWindow(u32 width, u32 height, str const &title,
                      OpenGL::WindowSettings const &settings) {
  if (mWindow) {
    throw Exception::ContextError("Window already exists. Currently, multiple "
                                  "windows are not supported.");
  }

  mWindow = std::make_shared<OpenGL::Window>(width, height, title, settings);

  if (!mGLADInitialized) {
    mWindow->setCurrentContext();
    OpenGL::initializeGLAD();
    mGLADInitialized = true;
    mWindow->unbind();
  }

  return mWindow;
}

void Context::tick(double fps) {
  if (!this->valid())
    return;

  mWindow->pollEvents();
  mClock->tick(fps);
}
} // namespace Terreate::Core
