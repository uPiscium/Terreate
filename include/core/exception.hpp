#pragma once

#include "../common/exception.hpp"
#include "../common/type.hpp"

namespace Terreate::Exception {
class CoreModuleError : public TerreateException {
public:
  CoreModuleError(str const &msg) : TerreateException(msg) {}
};

class ContextError : public CoreModuleError {
public:
  ContextError(str const &msg) : CoreModuleError(msg) {}
};

class EntityError : public CoreModuleError {
public:
  EntityError(str const &msg) : CoreModuleError(msg) {}
};
} // namespace Terreate::Exception
