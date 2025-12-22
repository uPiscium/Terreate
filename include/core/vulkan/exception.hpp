#pragma once

#include "common/exception.hpp"
#include "common/type.hpp"

namespace Terreate::Core::Vulkan {

class VulkanException : public TerreateException {
public:
  VulkanException(str const &message) : TerreateException(message) {}
};

class AllocatorException : public VulkanException {
public:
  AllocatorException(str const &message) : VulkanException(message) {}
};

class CommandException : public VulkanException {
public:
  CommandException(str const &message) : VulkanException(message) {}
};

class DebuggerException : public VulkanException {
public:
  DebuggerException(str const &message) : VulkanException(message) {}
};

class DeviceException : public VulkanException {
public:
  DeviceException(str const &message) : VulkanException(message) {}
};

class ImageException : public VulkanException {
public:
  ImageException(str const &message) : VulkanException(message) {}
};

class InstanceException : public VulkanException {
public:
  InstanceException(str const &message) : VulkanException(message) {}
};

class QueueException : public VulkanException {
public:
  QueueException(str const &message) : VulkanException(message) {}
};

class SwapchainException : public VulkanException {
public:
  SwapchainException(str const &message) : VulkanException(message) {}
};

} // namespace Terreate::Core::Vulkan
