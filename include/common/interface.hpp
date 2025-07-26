#pragma once

#include "type.hpp"

namespace Terreate::Interface {
class IController {
public:
  virtual ~IController() = default;
  virtual void update(float const &delta) = 0;
};

template <typename T>
concept Controller = extends<T, IController>;
} // namespace Terreate::Interface
