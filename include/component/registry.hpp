#pragma once

#include "interface.hpp"

namespace Terreate::Component {
class ComponentRegistry {
private:
  PROHIBIT_COPY_AND_ASSIGN(ComponentRegistry);

private:
  umap<typeindex, umap<UUID, shared<IComponent>>> mComponents;

public:
  ComponentRegistry() = default;
  ~ComponentRegistry() = default;

  template <Component T> shared<T> get(UUID const &entityID);
  template <Component T> umap<UUID, shared<T>> getAll();
  template <Component T> bool has(UUID const &entityID);
  template <Component T> void add(UUID const &entityID, shared<T> component);
  template <Component T> void remove(UUID const &entityID);
};
} // namespace Terreate::Component

// Implementation of the ComponentRegistry methods
namespace Terreate::Component {
template <Component T> shared<T> ComponentRegistry::get(UUID const &entityID) {
  typeindex type = typeindex(typeid(T));
  if (!mComponents.contains(type)) {
    return nullptr;
  }

  auto &entityComponents = mComponents[type];
  auto it = entityComponents.find(entityID);
  if (it == entityComponents.end()) {
    return nullptr;
  }

  return std::dynamic_pointer_cast<T>(it->second);
}

template <Component T> umap<UUID, shared<T>> ComponentRegistry::getAll() {
  typeindex type = typeindex(typeid(T));
  if (!mComponents.contains(type)) {
    return {};
  }

  auto &entityComponents = mComponents[type];
  umap<UUID, shared<T>> components;

  for (const auto &[id, component] : entityComponents) {
    shared<T> castedComponent = std::dynamic_pointer_cast<T>(component);
    if (castedComponent) {
      components.insert({id, castedComponent});
    }
  }

  return components;
}

template <Component T> bool ComponentRegistry::has(UUID const &entityID) {
  typeindex type = typeindex(typeid(T));
  if (!mComponents.contains(type)) {
    return false;
  }

  auto &entityComponents = mComponents[type];
  return entityComponents.contains(entityID);
}

template <Component T>
void ComponentRegistry::add(UUID const &entityID, shared<T> component) {
  if (!component)
    return;

  typeindex type = typeindex(typeid(T));
  if (!mComponents.contains(type)) {
    mComponents.insert({type, umap<UUID, shared<IComponent>>()});
  }

  auto &entityComponents = mComponents[type];
  if (entityComponents.contains(entityID)) {
    return; // Component already exists for this entity
  }
  entityComponents.insert({entityID, component});
}

template <Component T> void ComponentRegistry::remove(UUID const &entityID) {
  typeindex type = typeindex(typeid(T));
  if (!mComponents.contains(type)) {
    return; // No components of this type exist
  }

  auto &entityComponents = mComponents[type];
  auto it = entityComponents.find(entityID);
  if (it == entityComponents.end()) {
    return; // No component of this type for the specified entity
  }
  entityComponents.erase(it);
}
} // namespace Terreate::Component
