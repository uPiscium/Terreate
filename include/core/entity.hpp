#pragma once

#include "../common/event.hpp"
#include "../common/type.hpp"
#include "../common/uuid.hpp"

#include "../component/interface.hpp"
#include "../component/registry.hpp"

#include "exception.hpp"

namespace Terreate::Core {
class Entity;

typedef Event<Entity, shared<Component::IComponent>>
    EntityComponentEvent; // Event for entity component changes

class Entity {
private:
  UUID mID;
  shared<Component::ComponentRegistry> mComponentRegistry;

public:
  EntityComponentEvent onComponentAdded;
  EntityComponentEvent onComponentRemoved;

public:
  Entity(shared<Component::ComponentRegistry> registry)
      : mComponentRegistry(registry) {}
  ~Entity() = default;

  UUID const &getID() const { return mID; }
  template <Component::Component T> shared<T> getComponent() const;
  template <Component::Component T> void addComponent(shared<T> component);
  template <Component::Component T> void removeComponent();
  template <Component::Component T> void removeComponent(shared<T> component);
  operator UUID const &() const { return mID; }
};

class EntityManager {
private:
  PROHIBIT_COPY_AND_ASSIGN(EntityManager);

private:
  shared<Component::ComponentRegistry> mComponentRegistry;
  umap<UUID, shared<Entity>> mEntities;

public:
  EntityManager(shared<Component::ComponentRegistry> registry)
      : mComponentRegistry(registry) {}
  ~EntityManager() = default;

  shared<Entity> get(UUID const &id) const;

  bool has(UUID const &id) const { return mEntities.contains(id); }

  shared<Entity> create();
  void destroy(UUID const &id);
};
} // namespace Terreate::Core

// Implementation of EntityManager methods
namespace Terreate::Core {
template <Component::Component T> shared<T> Entity::getComponent() const {
  if (!mComponentRegistry) {
    throw Exception::EntityError("Component manager is not initialized.");
  }
  return mComponentRegistry->get<T>(mID);
}

template <Component::Component T>
void Entity::addComponent(shared<T> component) {
  if (!mComponentRegistry) {
    throw Exception::EntityError("Component manager is not initialized.");
  }
  mComponentRegistry->add<T>(mID, component);
}

template <Component::Component T> void Entity::removeComponent() {
  if (!mComponentRegistry) {
    throw Exception::EntityError("Component manager is not initialized.");
  }
  mComponentRegistry->remove<T>(mID);
}

template <Component::Component T>
void Entity::removeComponent(shared<T> component) {
  this->removeComponent<T>();
}
} // namespace Terreate::Core
