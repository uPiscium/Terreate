#pragma once

#include "../common/exception.hpp"
#include "../common/type.hpp"

namespace Terreate::Exception {
class ResourceModuleError : public TerreateException {
public:
  ResourceModuleError(str const &msg) : TerreateException(msg) {}
};
} // namespace Terreate::Exception
