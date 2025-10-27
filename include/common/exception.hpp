#pragma once

#include "type.hpp"

namespace Terreate::Exception {
class TerreateException : public std::exception {
private:
  str mMessage;

public:
  TerreateException(str const &message) noexcept : mMessage(message) {}
  virtual ~TerreateException() noexcept {}

  virtual const char *what() const noexcept override {
    return mMessage.c_str();
  }
};

class CommonModuleError : public TerreateException {
public:
  CommonModuleError(str const &message) noexcept : TerreateException(message) {}
};
} // namespace Terreate::Exception
