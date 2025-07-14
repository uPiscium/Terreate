#pragma once

#include "../common/type.hpp"
#include "../common/uuid.hpp"

#include "exception.hpp"

#include <typeindex>

namespace Terreate::Component {
class ComponentManager;

class IComponent {
public:
  virtual ~IComponent() = default;
  virtual UUID const &getID() const = 0;
};

template <typename T>
concept ComponentType = extends<T, IComponent>;

class ISystem {
public:
  virtual ~ISystem() = default;
  virtual UUID const &getID() const = 0;
  virtual void
  setComponentManager(shared<ComponentManager> componentManager) = 0;
  virtual void update(float const &delta) = 0;
};

template <typename T>
concept SystemType = extends<T, ISystem>;

class ComponentManager {
private:
  PROHIBIT_COPY_AND_ASSIGN(ComponentManager);

private:
  umap<std::type_index, umap<UUID, shared<IComponent>>> mComponents;

public:
  ComponentManager() = default;
  ~ComponentManager() = default;

  template <ComponentType T> shared<T> getComponent(UUID const &entityID) {
    std::type_index type = std::type_index(typeid(T));
    if (mComponents.find(type) == mComponents.end()) {
      return nullptr;
    }

    umap<UUID, shared<IComponent>> &entityComponents = mComponents[type];
    auto it = entityComponents.find(entityID);
    if (it == entityComponents.end()) {
      return nullptr;
    }

    return std::dynamic_pointer_cast<T>(it->second);
  }

  template <ComponentType T> umap<UUID, shared<T>> getComponents() {
    std::type_index type = std::type_index(typeid(T));
    if (mComponents.find(type) == mComponents.end()) {
      return {};
    }

    umap<UUID, shared<IComponent>> &entityComponents = mComponents[type];
    umap<UUID, shared<T>> components;

    for (const auto &[id, component] : entityComponents) {
      auto castedComponent = std::dynamic_pointer_cast<T>(component);
      if (castedComponent) {
        components.emplace(id, castedComponent);
      }
    }

    return components;
  }

  template <ComponentType T>
  void addComponent(UUID const &entityID, shared<T> component) {
    if (!component)
      return;

    std::type_index type = std::type_index(typeid(T));
    if (mComponents.find(type) == mComponents.end()) {
      mComponents.emplace(type, umap<UUID, shared<IComponent>>());
    }

    umap<UUID, shared<IComponent>> &entityComponents = mComponents[type];
    if (entityComponents.find(entityID) != entityComponents.end()) {
      throw Exception::ComponentModuleError(
          "Component already exists for this entity.");
    }

    entityComponents.emplace(entityID, component);
  }

  template <ComponentType T>
  void removeComponent(UUID const &entityID, shared<T> component) {
    if (!component)
      return;

    std::type_index type = std::type_index(typeid(T));
    if (mComponents.find(type) == mComponents.end()) {
      throw Exception::ComponentModuleError(
          "No components of this type exist.");
    }

    umap<UUID, shared<IComponent>> &entityComponents = mComponents[type];
    auto it = entityComponents.find(entityID);
    if (it == entityComponents.end()) {
      throw Exception::ComponentModuleError(
          "Component does not exist for this entity.");
    }

    entityComponents.erase(it);
  }
};
} // namespace Terreate::Component
