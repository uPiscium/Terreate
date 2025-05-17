#pragma once
#include <core/api.hpp>
#include <type.hpp>
#include <util/debugger.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;

class Instance {
private:
  VkInstance const mInstance;

public:
  Instance();
  Instance(VkInstance const instance);
  Instance(Instance const &instance);
  ~Instance();

  void attachDebugger(Util::IDebugger *debugger);

  operator VkInstance() const { return mInstance; }
};
} // namespace Terreate::Core
