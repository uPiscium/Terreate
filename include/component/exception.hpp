#pragma once

#include "../common/exception.hpp"
#include "../common/type.hpp"

namespace Terreate::Exception {
class ComponentModuleError : public TerreateException {
public:
  ComponentModuleError(str const &msg) : TerreateException(msg) {}
};
} // namespace Terreate::Exception
