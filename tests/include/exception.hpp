#pragma once
#include <exception>
#include <type.hpp>

namespace Terreate::Exception {
using namespace Terreate::Type;

class TerreateException : public std::exception {
private:
  str mMessage;

public:
  TerreateException(str const &message) : mMessage(message) {}

  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
  virtual str message() const noexcept { return mMessage; }
};

class NullReferenceException : public TerreateException {
public:
  NullReferenceException(str const &message) : TerreateException(message) {}
};

class GLFWInitializeFailure : public TerreateException {
public:
  GLFWInitializeFailure()
      : TerreateException("Failed to initialize GLFW library.") {}
};

class VulkanAPIError : public TerreateException {
public:
  VulkanAPIError(str const &message) : TerreateException(message) {}
};

class ValidationLayerNotSupported : public VulkanAPIError {
public:
  ValidationLayerNotSupported()
      : VulkanAPIError("Validation layer is not supported.") {}
};

class DebugMessengerFunctionLoadFailure : public VulkanAPIError {
public:
  DebugMessengerFunctionLoadFailure()
      : VulkanAPIError("Failed to load debug messenger functions.") {}
};

class DebugMessengerCreationFailure : public VulkanAPIError {
public:
  DebugMessengerCreationFailure()
      : VulkanAPIError("Failed to create debug messenger.") {}
};

class InstanceCreationFailure : public VulkanAPIError {
public:
  InstanceCreationFailure()
      : VulkanAPIError("Failed to create Vulkan instance.") {}
};
} // namespace Terreate::Exception
