#pragma once

#include "type.hpp"
#include "uuid.hpp"

namespace Terreate {
class IRegisterable {
public:
  virtual ~IRegisterable() = default;

  virtual UUID getID() const = 0;
  virtual bool operator==(IRegisterable const &other) const {
    return this->getID() == other.getID();
  }
};

template <typename T>
concept Registerable = extends<T, IRegisterable>;

class IRegistry {
public:
  virtual ~IRegistry() = default;
};

template <typename T>
concept Registry = extends<T, IRegistry>;
} // namespace Terreate
