#pragma once

#include "../common/type.hpp"
#include "../common/uuid.hpp"

namespace Terreate::Component {
class ComponentRegistry;

class IComponent {
public:
  virtual ~IComponent() = default;
  virtual UUID const &getID() const = 0;
};

template <typename T>
concept Component = extends<T, IComponent>;

class IComponentSystem {
public:
  virtual ~IComponentSystem() = default;
  virtual UUID const &getID() const = 0;
  virtual void inject(shared<ComponentRegistry> registry) = 0;
  virtual void update(float const &delta) = 0;
};

template <typename T>
concept ComponentSystem = extends<T, IComponentSystem>;
} // namespace Terreate::Component
