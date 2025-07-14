#pragma once

#include "../common/type.hpp"
#include "../common/uuid.hpp"
#include "../component/interface.hpp"

#include "exception.hpp"

namespace Terreate::Core {
class Entity;

typedef Event<Entity, shared<Component::IComponent>>
    EntityComponentEvent; // Event for entity component changes

class Entity {
private:
  UUID mID;
  shared<Component::ComponentManager> mComponentManager;

public:
  EntityComponentEvent onComponentAdded;
  EntityComponentEvent onComponentRemoved;

public:
  Entity(shared<Component::ComponentManager> componentManager)
      : mComponentManager(componentManager) {}
  ~Entity() = default;

  UUID const &getID() const { return mID; }
  template <Component::ComponentType T> shared<T> getComponent() const {
    if (!mComponentManager) {
      throw Exception::ComponentModuleError(
          "Component manager is not initialized.");
    }
    return mComponentManager->getComponent<T>(mID);
  }

  template <Component::ComponentType T> void addComponent(shared<T> component) {
    if (!mComponentManager) {
      throw Exception::ComponentModuleError(
          "Component manager is not initialized.");
    }
    mComponentManager->addComponent<T>(mID, component);
  }

  template <Component::ComponentType T> void removeComponent() {
    if (!mComponentManager) {
      throw Exception::ComponentModuleError(
          "Component manager is not initialized.");
    }
    mComponentManager->removeComponent<T>(mID);
  }

  operator UUID const &() const { return mID; }
};
} // namespace Terreate::Core
