#pragma once
#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <glfw/glfw3.h>

#include <exception.hpp>
#include <instance.hpp>
#include <type.hpp>

namespace Terreate {
using namespace Terreate::Types;

class TerreateContext {
private:
  vec<Core::Resource<API::VulkanInstance>> mInstances;

public:
  TerreateContext();
  ~TerreateContext();
};
} // namespace Terreate
