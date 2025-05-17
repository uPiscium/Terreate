#pragma once

namespace Terreate::Core {
class IDisposable {
public:
  virtual ~IDisposable() = default;
  virtual void dispose() = 0;
};
} // namespace Terreate::Core
