#pragma once

#include "common/exception.hpp"
#include "common/type.hpp"

namespace Terreate::Vulkan {

class VulkanException : public TerreateException {
public:
  VulkanException(str const &message) : TerreateException(message) {}
};

class InstanceException : public VulkanException {
public:
  InstanceException(str const &message) : VulkanException(message) {}
};

class DebuggerException : public VulkanException {
public:
  DebuggerException(str const &message) : VulkanException(message) {}
};

} // namespace Terreate::Vulkan
