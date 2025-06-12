#pragma once
#include "../common/type.hpp"

namespace Terreate::Vulkan {
static char const *VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";
static Type::vec<char const *> const VALIDATION_LAYERS = {
    VK_LAYER_KHRONOS_VALIDATION};
static Type::umap<VkPhysicalDeviceType, int> const SCORES = {
    {VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU, 10000},
    {VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU, 1000},
    {VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU, 100},
    {VK_PHYSICAL_DEVICE_TYPE_CPU, 10},
    {VK_PHYSICAL_DEVICE_TYPE_OTHER, 0}};
static Type::vec<char const *> const EXTS = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

inline PFN_vkCreateDebugUtilsMessengerEXT trCreateDebugUtilsMessengerEXT =
    nullptr;
inline PFN_vkDestroyDebugUtilsMessengerEXT trDestroyDebugUtilsMessengerEXT =
    nullptr;
} // namespace Terreate::Vulkan
