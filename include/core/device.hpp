#pragma once

#include <core/IDisposable.hpp>
#include <core/api.hpp>
#include <type.hpp>

namespace Terreate::Core {
using namespace Terreate::Type;

class Device : public IDisposable {
private:
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;

private:
  int rateDevice(VkPhysicalDevice device) const;
  void pickPhysicalDevice(VkInstance instance);

public:
  Device(VkInstance instance);
  ~Device();

  void dispose() override;
};
} // namespace Terreate::Core
