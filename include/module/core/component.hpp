#pragma once

#include "../common/uuid.hpp"

namespace Terreate::Core {
class IComponent {
public:
  virtual ~IComponent() = default;

  virtual UUID const &getUUID() const = 0;
  virtual void update(double delta) = 0;
};

class IComponentRegistry {
public:
  virtual ~IComponentRegistry() = default;

  virtual void registerComponent(IComponent *component) = 0;
  virtual void unregisterComponent(IComponent *component) = 0;
  virtual void update(double delta) = 0;
};
} // namespace Terreate::Core
