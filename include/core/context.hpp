#pragma once

#include "../common/clock.hpp"
#include "../common/interface.hpp"

#include "../sdl/event.hpp"
#include "../sdl/window.hpp"

#include "entity.hpp"

namespace Terreate::Core {
class Context {
private:
  bool mGLADInitialized = false;
  shared<Clock> mClock;
  shared<SDL::Window> mWindow;
  shared<EntityManager> mEntityManager;
  umap<typeindex, shared<Interface::IController>> mControllers;

  shared<SDL::ObjectRegistry> mSDLObjectRegistry = nullptr;
  shared<SDL::EventHandler> mEventHandler = nullptr;

public:
  Context();
  ~Context();

  double getTime() const { return mClock->getCurrentRuntime(); }
  shared<Clock> getClock() const { return mClock; }
  shared<SDL::EventHandler> getEventHandler() const { return mEventHandler; }
  template <Interface::Controller T>
  shared<T> getController(bool create = false);

  shared<SDL::Window> createWindow(u32 width, u32 height, str const &title,
                                   bool autoCloseOnEvent = true);
  shared<Entity> createEntity() { return mEntityManager->create(); }
  template <Interface::Controller T> shared<T> createController();

  bool valid() const { return mWindow && !mWindow->isClosed(); }
  void tick(double fps);

  operator bool() const { return mWindow && !mWindow->isClosed(); }
};
} // namespace Terreate::Core

// Implementation of createController
namespace Terreate::Core {
template <Interface::Controller T>
shared<T> Context::getController(bool create) {
  typeindex type = typeid(T);
  if (mControllers.contains(type)) {
    return std::static_pointer_cast<T>(mControllers[type]);
  }

  if (create) {
    return this->createController<T>();
  }

  return nullptr;
}

template <Interface::Controller T> shared<T> Context::createController() {
  typeindex type = typeid(T);
  if (mControllers.contains(type)) {
    return std::static_pointer_cast<T>(mControllers[type]);
  }

  shared<T> controller = std::make_shared<T>();
  mControllers.insert({type, controller});
  return controller;
}
} // namespace Terreate::Core
