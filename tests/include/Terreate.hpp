#pragma once

#include <context.hpp>
#include <instance.hpp>

namespace Terreate {
template <typename T> using HubPointer = Terreate::Core::HubPointer<T>;
template <typename T> using RefPointer = Terreate::Core::RefPointer<T>;

typedef Core::TerreateContext Context;
typedef Core::VulkanInstance Instance;

} // namespace Terreate
