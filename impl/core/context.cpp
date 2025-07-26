#include <component/controller.hpp>

#include <core/context.hpp>
#include <core/exception.hpp>

#include <resource/resource.hpp>

#include <sdl/sdl.hpp>

namespace Terreate::Core {
Context::Context() {
  SDL::initializeSDL();
  mClock = std::make_shared<Clock>();
  mSDLObjectRegistry = std::make_shared<SDL::ObjectRegistry>();
  mEventHandler = SDL::EventHandler::create(mSDLObjectRegistry);

  auto compController =
      this->createController<Component::ComponentController>();

  mEntityManager =
      std::make_shared<EntityManager>(compController->getRegistry());
}

Context::~Context() {
  if (mWindow) {
    mWindow->close();
    mWindow.reset();
  }
  mEntityManager.reset();
  for (auto &controller : mControllers) {
    controller.second.reset();
  }
  mEventHandler.reset();
  mSDLObjectRegistry.reset();
  mClock.reset();
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
  if (mSDLObjectRegistry->hasMouse(0)) {
    mouse = mSDLObjectRegistry->getMouse(0);
  } else {
    mouse = std::make_shared<SDL::Mouse>(0);
    mSDLObjectRegistry->registerMouse(0, mouse);
  }
  mWindow = std::make_shared<SDL::Window>(width, height, title, mouse);
  mSDLObjectRegistry->registerWindow(mWindow->getId(), mWindow);

  if (autoCloseOnEvent) {
    mEventHandler->window.onWindowCloseRequested.subscribe(
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
  for (auto &controller : mControllers) {
    controller.second->update(mClock->getDeltaTime());
  }
  mClock->tick(fps);
}
} // namespace Terreate::Core
