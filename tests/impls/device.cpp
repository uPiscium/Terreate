// #include <device.hpp>

// namespace Terreate::Core {
// QueueFamilyIndices::operator set<u32>() const {
//   return {graphicsFamily.value(), presentFamily.value()};
// }

// QueueFamilyIndices::operator bool() const {
//   return graphicsFamily.has_value() && presentFamily.has_value();
// }

// QueueFamilyIndices
// LogicalDevice::findQueueFamilies(VkPhysicalDevice device,
//                                  VkSurfaceKHR surface) const {
//   QueueFamilyIndices indices;
//   uint32_t i = 0;
//   for (auto const &queueFamily : API::getQueueFamilyProperties(device)) {
//     if (queueFamily.queueCount > 0 &&
//         (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
//       indices.graphicsFamily = i;
//     }

//     VkBool32 presentSupport = false;
//     vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface,
//     &presentSupport); if (presentSupport) {
//       indices.presentFamily = i;
//     }

//     if (indices) {
//       break;
//     }

//     ++i;
//   }
//   return indices;
// }

// int LogicalDevice::rateDevice(VkPhysicalDevice device,
//                               VkSurfaceKHR surface) const {
//   VkPhysicalDeviceProperties deviceProperties;
//   vkGetPhysicalDeviceProperties(device, &deviceProperties);

//   int score = API::SCORES.at(deviceProperties.deviceType);

//   QueueFamilyIndices indices = this->findQueueFamilies(device, surface);
//   if (indices) {
//     return 0;
//   }

//   score += deviceProperties.limits.maxImageDimension2D;

//   return score;
// }

// void LogicalDevice::pickPhysicalDevice(VkInstance instance,
//                                        VkSurfaceKHR surface) {
//   mmap<int, VkPhysicalDevice> deviceMap;

//   for (auto &device : API::getPhysicalDevices(instance)) {
//     if (!API::isDeviceExtsSupported(device)) {
//       continue;
//     }

//     auto score = this->rateDevice(device, surface);
//     if (score <= 0) {
//       continue;
//     }

//     API::SwapchainSupport support = API::getSwapchainSupport(device,
//     surface); if (support.formats.empty() || support.presentModes.empty()) {
//       continue;
//     }

//     deviceMap.insert(std::make_pair(score, device));
//   }

//   if (deviceMap.empty()) {
//     throw Exception::NoDeviceFound();
//   }

//   mPhysicalDevice = deviceMap.rbegin()->second;
// }

// LogicalDevice::LogicalDevice(VkInstance instance, VkSurfaceKHR surface) {
//   this->pickPhysicalDevice(instance, surface);
//   QueueFamilyIndices indices =
//       this->findQueueFamilies(mPhysicalDevice, surface);
//   set<u32> queueFamily = indices;
//   vec<VkDeviceQueueCreateInfo> queueCreateInfos;
//   float queuePriority = 1.0f;

//   for (auto const &family : queueFamily) {
//     VkDeviceQueueCreateInfo queueCreateInfo = {};
//     queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//     queueCreateInfo.queueFamilyIndex = family;
//     queueCreateInfo.queueCount = 1;
//     queueCreateInfo.pQueuePriorities = &queuePriority;
//     queueCreateInfos.push_back(queueCreateInfo);
//   }

//   VkPhysicalDeviceFeatures deviceFeatures{};
//   VkDeviceCreateInfo createInfo{};
//   createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//   createInfo.queueCreateInfoCount =
//       static_cast<uint32_t>(queueCreateInfos.size());
//   createInfo.pQueueCreateInfos = queueCreateInfos.data();
//   createInfo.pEnabledFeatures = &deviceFeatures;
//   createInfo.enabledExtensionCount = static_cast<uint32_t>(API::EXTS.size());
//   createInfo.ppEnabledExtensionNames = API::EXTS.data();

// #ifdef TERREATE_DEBUG_BUILD
//   createInfo.enabledLayerCount =
//       static_cast<uint32_t>(API::VALIDATION_LAYERS.size());
//   createInfo.ppEnabledLayerNames = API::VALIDATION_LAYERS.data();
// #else
//   createInfo.enabledLayerCount = 0;
// #endif

//   if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) !=
//       VK_SUCCESS) {
//     throw std::runtime_error("Failed to create logical device!");
//   }

//   vkGetDeviceQueue(mDevice, indices.graphicsFamily.value(), 0,
//   &mGraphicsQueue); vkGetDeviceQueue(mDevice, indices.presentFamily.value(),
//   0, &mPresentQueue);
// }

// LogicalDevice::~LogicalDevice() {
//   if (mDevice != VK_NULL_HANDLE) {
//     vkDestroyDevice(mDevice, nullptr);
//   }
// }

// QueueFamilyIndices LogicalDevice::getQueueFamilies(VkSurfaceKHR surface)
// const {
//   return this->findQueueFamilies(mPhysicalDevice, surface);
// }

// LogicalDevice::operator VkPhysicalDevice() const { return mPhysicalDevice; }

// LogicalDevice::operator VkDevice() const { return mDevice; }

// } // namespace Terreate::Core
