#pragma once

#include "../common/exception.hpp"
#include "../common/type.hpp"

namespace Terreate::Exception {
class ResourceModuleError : public TerreateException {
public:
  ResourceModuleError(str const &msg) : TerreateException(msg) {}
};

class ShaderResourceError : public ResourceModuleError {
public:
  ShaderResourceError(str const &msg)
      : ResourceModuleError("Shader Error: " + msg) {}
};
} // namespace Terreate::Exception
