#pragma once

#include "../common/interface.hpp"

#include "interface.hpp"
#include "registry.hpp"

namespace Terreate::Component {
class ComponentController : public Interface::IController {
private:
  PROHIBIT_COPY_AND_ASSIGN(ComponentController);

private:
  shared<ComponentRegistry> mComponentRegistry;
  umap<typeindex, shared<IComponentSystem>> mSystems;

public:
  ComponentController()
      : mComponentRegistry(std::make_shared<ComponentRegistry>()) {}
  ~ComponentController() = default;

  shared<ComponentRegistry> getRegistry() const { return mComponentRegistry; }

  template <ComponentSystem T> shared<T> getSytem(bool create = false) const;
  template <ComponentSystem T> bool hasSystem() const;
  template <ComponentSystem T> shared<T> createSystem();
  template <ComponentSystem T> void destroySystem();

  void update(float const &delta) override;
};
} // namespace Terreate::Component

// Implementation of the ComponentController methods.
namespace Terreate::Component {
template <ComponentSystem T>
shared<T> ComponentController::getSytem(bool create) const {
  typeindex type = typeindex(typeid(T));
  if (mSystems.contains(type)) {
    return std::dynamic_pointer_cast<T>(mSystems.at(type));
  }

  if (create) {
    return this->createSystem<T>();
  }

  return nullptr;
}

template <ComponentSystem T> bool ComponentController::hasSystem() const {
  typeindex type = typeindex(typeid(T));
  return mSystems.contains(type);
}

template <ComponentSystem T> shared<T> ComponentController::createSystem() {
  typeindex type = typeindex(typeid(T));
  if (mSystems.contains(type)) {
    return std::dynamic_pointer_cast<T>(mSystems.at(type));
  }

  shared<T> system = std::make_shared<T>();
  system->inject(mComponentRegistry);
  mSystems.insert({type, system});
  return system;
}

template <ComponentSystem T> void ComponentController::destroySystem() {
  typeindex type = typeindex(typeid(T));
  if (!mSystems.contains(type)) {
    return;
  }
  mSystems.erase(type);
}
} // namespace Terreate::Component
