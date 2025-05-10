#pragma once
#include <exception>
#include <utils/type.hpp>

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

class FatalException : public TerreateException {
public:
  FatalException(str const &message) : TerreateException(message) {}
};

class VulkanAPIException : public TerreateException {
public:
  VulkanAPIException(str const &message) : TerreateException(message) {}
};

class InstanceCreationFailure : public VulkanAPIException {
public:
  InstanceCreationFailure(str const &message)
      : VulkanAPIException("Instance creation failure: " + message) {}
};

} // namespace Terreate::Exception
