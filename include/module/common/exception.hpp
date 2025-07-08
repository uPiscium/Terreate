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

class ResourceException : public TerreateException {
public:
  ResourceException(str const &message) noexcept : TerreateException(message) {}
};

class CommonModuleError : public TerreateException {
public:
  CommonModuleError(str const &message) noexcept : TerreateException(message) {}
};

class ExecutorError : public CommonModuleError {
public:
  ExecutorError(str const &message) noexcept : CommonModuleError(message) {}
};

class TaskError : public CommonModuleError {
public:
  TaskError(str const &message) noexcept : CommonModuleError(message) {}
};

class NullReferenceException : public CommonModuleError {
public:
  NullReferenceException(str const &message) noexcept
      : CommonModuleError(message) {}
};

class NullObjectException : public CommonModuleError {
public:
  NullObjectException() : CommonModuleError("Null object") {}
};

class NotImplementedException : public CommonModuleError {
public:
  NotImplementedException() noexcept : CommonModuleError("Not implemented") {}
  NotImplementedException(str const &message) noexcept
      : CommonModuleError(message) {}
};

// class AudioException : public TerreateException {
// public:
//   AudioException(str const &message) noexcept : TerreateException(message) {}
// };

// class OpenALException : public AudioException {
// public:
//   OpenALException(str const &message) noexcept : AudioException(message) {}
// };

// class SourceError : public AudioException {
// public:
//   SourceError(str const &message) noexcept : AudioException(message) {}
// };

} // namespace Terreate::Exception
