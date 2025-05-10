#pragma once
#include <utils/type.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;

class Instance {
private:
  VkInstance mInstance = VK_NULL_HANDLE;

public:
  Instance(char const *appName, vec<char const *> const &exts);
  ~Instance();

  operator VkInstance() const { return mInstance; }
};
} // namespace Terreate::Core
