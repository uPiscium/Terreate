#include "common/bitflag.hpp"
#include "core/vulkan/device.hpp"

namespace Terreate::Vulkan {

VkSampleCountFlagBits PhysicalDevice::getMaxUsableSampleCount() const {
  VkPhysicalDeviceProperties physicalDeviceProperties;
  vkGetPhysicalDeviceProperties(mHandle, &physicalDeviceProperties);

  VkSampleCountFlags counts =
      physicalDeviceProperties.limits.framebufferColorSampleCounts &
      physicalDeviceProperties.limits.framebufferDepthSampleCounts;

  VkSampleCountFlagBits sampleCounts[] = {
      VK_SAMPLE_COUNT_64_BIT, VK_SAMPLE_COUNT_32_BIT, VK_SAMPLE_COUNT_16_BIT,
      VK_SAMPLE_COUNT_8_BIT,  VK_SAMPLE_COUNT_4_BIT,  VK_SAMPLE_COUNT_2_BIT,
      VK_SAMPLE_COUNT_1_BIT};

  for (auto const count : sampleCounts) {
    if (counts & count) {
      return count;
    }
  }

  return VK_SAMPLE_COUNT_1_BIT;
}

PhysicalDevice::PhysicalDevice(VkPhysicalDevice physicalDevice,
                               shared<SDL::Window> window)
    : mWindow(window), mHandle(physicalDevice) {
  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyCount, nullptr);
  mQueueFamilies.resize(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(mHandle, &queueFamilyCount,
                                           mQueueFamilies.data());

  u32 extensionCount = 0;
  vkEnumerateDeviceExtensionProperties(mHandle, nullptr, &extensionCount,
                                       nullptr);
  mSupportedExtensions.resize(extensionCount);
  vkEnumerateDeviceExtensionProperties(mHandle, nullptr, &extensionCount,
                                       mSupportedExtensions.data());

  vkGetPhysicalDeviceProperties(mHandle, &mProperties);
  vkGetPhysicalDeviceFeatures(mHandle, &mSupportedFeatures);
  vkGetPhysicalDeviceMemoryProperties(mHandle, &mMemoryProperties);

  mMaxSampleCount = this->getMaxUsableSampleCount();
}

uset<u32> PhysicalDevice::getQueue(VkQueueFlags const &flags) const {
  uset<u32> indices;
  for (u32 i = 0; i < mQueueFamilies.size(); ++i) {
    auto const &queueFamily = mQueueFamilies[i];
    if (queueFamily.queueCount <= 0) {
      continue;
    }

    if ((queueFamily.queueFlags & flags) == flags) {
      indices.insert(i);
    }
  }

  return indices;
}

VkSampleCountFlagBits const &PhysicalDevice::getMaxSampleCount() const {
  return mMaxSampleCount;
}

vec<VkQueueFamilyProperties> const &PhysicalDevice::getQueueFamilies() const {
  return mQueueFamilies;
}

vec<VkExtensionProperties> const &
PhysicalDevice::getSupportedExtensions() const {
  return mSupportedExtensions;
}

VkPhysicalDeviceProperties const &PhysicalDevice::getProperties() const {
  return mProperties;
}

VkPhysicalDeviceFeatures const &PhysicalDevice::getSupportedFeatures() const {
  return mSupportedFeatures;
}

VkPhysicalDeviceMemoryProperties const &
PhysicalDevice::getMemoryProperties() const {
  return mMemoryProperties;
}

VkFormatProperties
PhysicalDevice::getFormatProperties(VkFormat const &format) const {
  VkFormatProperties formatProperties;
  vkGetPhysicalDeviceFormatProperties(mHandle, format, &formatProperties);
  return formatProperties;
}

VkSurfaceCapabilitiesKHR PhysicalDevice::getSurfaceCapabilities() const {
  VkSurfaceCapabilitiesKHR capabilities;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(mHandle, *mWindow, &capabilities);
  return capabilities;
}

vec<VkSurfaceFormatKHR> PhysicalDevice::getSurfaceFormats() const {
  u32 formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(mHandle, *mWindow, &formatCount,
                                       nullptr);
  vec<VkSurfaceFormatKHR> formats(formatCount);
  vkGetPhysicalDeviceSurfaceFormatsKHR(mHandle, *mWindow, &formatCount,
                                       formats.data());
  return formats;
}

vec<VkPresentModeKHR> PhysicalDevice::getPresentModes() const {
  u32 presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(mHandle, *mWindow,
                                            &presentModeCount, nullptr);
  vec<VkPresentModeKHR> presentModes(presentModeCount);
  vkGetPhysicalDeviceSurfacePresentModesKHR(
      mHandle, *mWindow, &presentModeCount, presentModes.data());
  return presentModes;
}

u32 PhysicalDevice::findMemoryType(u32 typeFilter,
                                   VkMemoryPropertyFlags properties) const {
  for (u32 i = 0; i < mMemoryProperties.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) &&
        (mMemoryProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
      return i;
    }
  }

  throw DeviceException("Failed to find suitable memory type.");
}

bool PhysicalDevice::hasQueueSupport(
    uset<QueueType> const &requiredQueues) const {
  for (auto const &queue : requiredQueues) {
    if (this->getQueue((VkQueueFlags)queue).empty()) {
      return false;
    }
  }
  return true;
}

bool PhysicalDevice::hasSurfaceSupport(u32 queueFamilyIndex) const {
  VkBool32 presentSupport = false;
  vkGetPhysicalDeviceSurfaceSupportKHR(mHandle, queueFamilyIndex, *mWindow,
                                       &presentSupport);
  return presentSupport;
}

bool PhysicalDevice::hasExtensionSupport(str const &extensionName) const {
  for (auto const &extension : mSupportedExtensions) {
    if (extensionName == extension.extensionName) {
      return true;
    }
  }
  return false;
}

bool PhysicalDevice::hasExtensionSupport(vec<str> const &extensionNames) const {
  for (auto const &extensionName : extensionNames) {
    if (!this->hasExtensionSupport(extensionName)) {
      return false;
    }
  }
  return true;
}

bool PhysicalDevice::hasSwapChainSupport() const {
  u32 formatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR(mHandle, *mWindow, &formatCount,
                                       nullptr);
  if (formatCount == 0) {
    return false;
  }

  u32 presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR(mHandle, *mWindow,
                                            &presentModeCount, nullptr);
  if (presentModeCount == 0) {
    return false;
  }

  return true;
}

PhysicalDevice::operator VkPhysicalDevice() const { return mHandle; }

shared<PhysicalDevice> PhysicalDevice::create(VkPhysicalDevice physicalDevice,
                                              shared<SDL::Window> window) {
  PhysicalDevice *device = new PhysicalDevice(physicalDevice, window);
  return shared<PhysicalDevice>(device);
}

i32 PhysicalDevice::rateDevice(shared<PhysicalDevice> physicalDevice) {
  int score = 0;
  VkPhysicalDeviceProperties properties = physicalDevice->getProperties();

  // Discrete GPUs have a significant performance advantage
  if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  score += properties.limits.maxImageDimension2D;
  score += properties.limits.maxImageDimensionCube;
  score += properties.limits.maxImageArrayLayers;
  score += properties.limits.maxMemoryAllocationCount;
  score += properties.limits.maxComputeWorkGroupInvocations;
  score += properties.limits.maxComputeSharedMemorySize;

  return score;
}

shared<PhysicalDevice>
PhysicalDevice::pick(shared<Instance> instance, shared<SDL::Window> window,
                     uset<QueueType> const &requiredQueues,
                     vec<str> const &requiredExtensions,
                     DeviceRateFunction func) {
  u32 deviceCount = 0;
  vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw DeviceException("Failed to find GPUs with Vulkan support.");
  }
  vec<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());

  i32 bestScore = -1;
  shared<PhysicalDevice> bestDevice = nullptr;
  for (auto const &device : devices) {
    shared<PhysicalDevice> physicalDevice =
        PhysicalDevice::create(device, window);

    if (!physicalDevice->hasQueueSupport(requiredQueues)) {
      continue;
    }

    if (!physicalDevice->hasExtensionSupport(requiredExtensions)) {
      continue;
    }

    if (requiredQueues.contains(QueueType::GRAPHICS)) {
      uset<u32> graphicsQueueIndex =
          physicalDevice->getQueue((VkQueueFlags)QueueType::GRAPHICS);

      bool hasSurfaceSupport = false;
      for (u32 const &index : graphicsQueueIndex) {
        if (physicalDevice->hasSurfaceSupport(index)) {
          hasSurfaceSupport = true;
          break;
        }
      }
      if (!hasSurfaceSupport) {
        continue;
      }

      if (!physicalDevice->hasSwapChainSupport()) {
        continue;
      }
    }

    i32 deviceScore = func(physicalDevice);
    if (deviceScore > bestScore) {
      bestScore = deviceScore;
      bestDevice = physicalDevice;
    }
  }

  if (bestDevice == nullptr) {
    throw DeviceException("Failed to find a suitable GPU.");
  }

  return bestDevice;
}

Device::Device(shared<Instance> instance, shared<SDL::Window> window,
               shared<PhysicalDevice> physicalDevice,
               vec<DeviceFeatures> const &requiredFeatures,
               uset<QueueType> const &requiredQueues,
               vec<str> const &requiredExtensions)
    : mInstance(instance), mPhysicalDevice(physicalDevice) {
  float queuePriority = 1.0f;
  vec<VkDeviceQueueCreateInfo> queueCreateInfos;

  for (auto const &queue : requiredQueues) {
    uset<u32> indices = physicalDevice->getQueue((VkQueueFlags)queue);
    if (indices.empty()) {
      throw DeviceException("Failed to find a suitable queue family.");
    }

    for (auto const &index : indices) {
      bool found = true;
      for (auto const &pair : mQueueFamilyIndices) {
        if (pair.second == index) {
          found = false;
          break;
        }
      }

      if (found) {
        mQueueFamilyIndices.insert({queue, index});
        break;
      }
    }
  }

  for (auto const &pair : mQueueFamilyIndices) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = pair.second;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  Bitflag<DeviceFeatures> featuresFlag;
  for (auto const &feature : requiredFeatures) {
    featuresFlag |= feature;
  }
  VkPhysicalDeviceFeatures deviceFeatures{};
  deviceFeatures.robustBufferAccess =
      (featuresFlag & DeviceFeatures::ROBUST_BUFFER_ACCESS) ? VK_TRUE
                                                            : VK_FALSE;
  deviceFeatures.fullDrawIndexUint32 =
      (featuresFlag & DeviceFeatures::FULL_DRAW_INDEX_UINT32) ? VK_TRUE
                                                              : VK_FALSE;
  deviceFeatures.imageCubeArray =
      (featuresFlag & DeviceFeatures::IMAGE_CUBE_ARRAY) ? VK_TRUE : VK_FALSE;
  deviceFeatures.independentBlend =
      (featuresFlag & DeviceFeatures::INDEPENDENT_BLEND) ? VK_TRUE : VK_FALSE;
  deviceFeatures.geometryShader =
      (featuresFlag & DeviceFeatures::GEOMETRY_SHADER) ? VK_TRUE : VK_FALSE;
  deviceFeatures.tessellationShader =
      (featuresFlag & DeviceFeatures::TESSELLATION_SHADER) ? VK_TRUE : VK_FALSE;
  deviceFeatures.sampleRateShading =
      (featuresFlag & DeviceFeatures::SAMPLE_RATE_SHADING) ? VK_TRUE : VK_FALSE;
  deviceFeatures.dualSrcBlend =
      (featuresFlag & DeviceFeatures::DUAL_SRC_BLEND) ? VK_TRUE : VK_FALSE;
  deviceFeatures.logicOp =
      (featuresFlag & DeviceFeatures::LOGIC_OP) ? VK_TRUE : VK_FALSE;
  deviceFeatures.multiDrawIndirect =
      (featuresFlag & DeviceFeatures::MULTI_DRAW_INDIRECT) ? VK_TRUE : VK_FALSE;
  deviceFeatures.drawIndirectFirstInstance =
      (featuresFlag & DeviceFeatures::DRAW_INDIRECT_FIRST_INSTANCE) ? VK_TRUE
                                                                    : VK_FALSE;
  deviceFeatures.depthClamp =
      (featuresFlag & DeviceFeatures::DEPTH_CLAMP) ? VK_TRUE : VK_FALSE;
  deviceFeatures.depthBiasClamp =
      (featuresFlag & DeviceFeatures::DEPTH_BIAS_CLAMP) ? VK_TRUE : VK_FALSE;
  deviceFeatures.fillModeNonSolid =
      (featuresFlag & DeviceFeatures::FILL_MODE_NON_SOLID) ? VK_TRUE : VK_FALSE;
  deviceFeatures.depthBounds =
      (featuresFlag & DeviceFeatures::DEPTH_BOUNDS) ? VK_TRUE : VK_FALSE;
  deviceFeatures.wideLines =
      (featuresFlag & DeviceFeatures::WIDE_LINES) ? VK_TRUE : VK_FALSE;
  deviceFeatures.largePoints =
      (featuresFlag & DeviceFeatures::LARGE_POINTS) ? VK_TRUE : VK_FALSE;
  deviceFeatures.alphaToOne =
      (featuresFlag & DeviceFeatures::ALPHA_TO_ONE) ? VK_TRUE : VK_FALSE;
  deviceFeatures.multiViewport =
      (featuresFlag & DeviceFeatures::MULTI_VIEWPORT) ? VK_TRUE : VK_FALSE;
  deviceFeatures.samplerAnisotropy =
      (featuresFlag & DeviceFeatures::SAMPLER_ANISOTROPY) ? VK_TRUE : VK_FALSE;
  deviceFeatures.textureCompressionETC2 =
      (featuresFlag & DeviceFeatures::TEXTURE_COMPRESSIONETC2) ? VK_TRUE
                                                               : VK_FALSE;
  deviceFeatures.textureCompressionASTC_LDR =
      (featuresFlag & DeviceFeatures::TEXTURE_COMPRESSION_ASTC_LDR) ? VK_TRUE
                                                                    : VK_FALSE;
  deviceFeatures.textureCompressionBC =
      (featuresFlag & DeviceFeatures::TEXTURE_COMPRESSIONBC) ? VK_TRUE
                                                             : VK_FALSE;
  deviceFeatures.occlusionQueryPrecise =
      (featuresFlag & DeviceFeatures::OCCLUSION_QUERY_PRECISE) ? VK_TRUE
                                                               : VK_FALSE;
  deviceFeatures.pipelineStatisticsQuery =
      (featuresFlag & DeviceFeatures::PIPELINE_STATISTICS_QUERY) ? VK_TRUE
                                                                 : VK_FALSE;
  deviceFeatures.vertexPipelineStoresAndAtomics =
      (featuresFlag & DeviceFeatures::VERTEX_PIPELINE_STORES_AND_ATOMICS)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.fragmentStoresAndAtomics =
      (featuresFlag & DeviceFeatures::FRAGMENT_STORES_AND_ATOMICS) ? VK_TRUE
                                                                   : VK_FALSE;
  deviceFeatures.shaderTessellationAndGeometryPointSize =
      (featuresFlag &
       DeviceFeatures::SHADER_TESSELLATION_AND_GEOMETRY_POINT_SIZE)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderImageGatherExtended =
      (featuresFlag & DeviceFeatures::SHADER_IMAGE_GATHER_EXTENDED) ? VK_TRUE
                                                                    : VK_FALSE;
  deviceFeatures.shaderStorageImageExtendedFormats =
      (featuresFlag & DeviceFeatures::SHADER_STORAGE_IMAGE_EXTENDED_FORMATS)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderStorageImageMultisample =
      (featuresFlag & DeviceFeatures::SHADER_STORAGE_IMAGE_MULTISAMPLE)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderStorageImageReadWithoutFormat =
      (featuresFlag & DeviceFeatures::SHADER_STORAGE_IMAGE_READ_WITHOUT_FORMAT)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderStorageImageWriteWithoutFormat =
      (featuresFlag & DeviceFeatures::SHADER_STORAGE_IMAGE_WRITE_WITHOUT_FORMAT)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderUniformBufferArrayDynamicIndexing =
      (featuresFlag &
       DeviceFeatures::SHADER_UNIFORM_BUFFER_ARRAY_DYNAMIC_INDEXING)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderSampledImageArrayDynamicIndexing =
      (featuresFlag &
       DeviceFeatures::SHADER_SAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderStorageBufferArrayDynamicIndexing =
      (featuresFlag &
       DeviceFeatures::SHADER_STORAGE_BUFFER_ARRAY_DYNAMIC_INDEXING)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderStorageImageArrayDynamicIndexing =
      (featuresFlag &
       DeviceFeatures::SHADER_STORAGE_IMAGE_ARRAY_DYNAMIC_INDEXING)
          ? VK_TRUE
          : VK_FALSE;
  deviceFeatures.shaderClipDistance =
      (featuresFlag & DeviceFeatures::SHADER_CLIP_DISTANCE) ? VK_TRUE
                                                            : VK_FALSE;
  deviceFeatures.shaderCullDistance =
      (featuresFlag & DeviceFeatures::SHADER_CULL_DISTANCE) ? VK_TRUE
                                                            : VK_FALSE;
  deviceFeatures.shaderFloat64 =
      (featuresFlag & DeviceFeatures::SHADER_FLOAT64) ? VK_TRUE : VK_FALSE;
  deviceFeatures.shaderInt64 =
      (featuresFlag & DeviceFeatures::SHADER_INT64) ? VK_TRUE : VK_FALSE;
  deviceFeatures.shaderInt16 =
      (featuresFlag & DeviceFeatures::SHADER_INT16) ? VK_TRUE : VK_FALSE;
  deviceFeatures.shaderResourceResidency =
      (featuresFlag & DeviceFeatures::SHADER_RESOURCE_RESIDENCY) ? VK_TRUE
                                                                 : VK_FALSE;
  deviceFeatures.shaderResourceMinLod =
      (featuresFlag & DeviceFeatures::SHADER_RESOURCE_MIN_LOD) ? VK_TRUE
                                                               : VK_FALSE;
  deviceFeatures.sparseBinding =
      (featuresFlag & DeviceFeatures::SPARSE_BINDING) ? VK_TRUE : VK_FALSE;
  deviceFeatures.sparseResidencyBuffer =
      (featuresFlag & DeviceFeatures::SPARSE_RESIDENCY_BUFFER) ? VK_TRUE
                                                               : VK_FALSE;
  deviceFeatures.sparseResidencyImage2D =
      (featuresFlag & DeviceFeatures::SPARSE_RESIDENCY_IMAGE_2D) ? VK_TRUE
                                                                 : VK_FALSE;
  deviceFeatures.sparseResidencyImage3D =
      (featuresFlag & DeviceFeatures::SPARSE_RESIDENCY_IMAGE_3D) ? VK_TRUE
                                                                 : VK_FALSE;
  deviceFeatures.sparseResidency2Samples =
      (featuresFlag & DeviceFeatures::SPARSE_RESIDENCY_2_SAMPLES) ? VK_TRUE
                                                                  : VK_FALSE;
  deviceFeatures.sparseResidency4Samples =
      (featuresFlag & DeviceFeatures::SPARSE_RESIDENCY_4_SAMPLES) ? VK_TRUE
                                                                  : VK_FALSE;
  deviceFeatures.sparseResidency8Samples =
      (featuresFlag & DeviceFeatures::SPARSE_RESIDENCY_8_SAMPLES) ? VK_TRUE
                                                                  : VK_FALSE;
  deviceFeatures.sparseResidency16Samples =
      (featuresFlag & DeviceFeatures::SPARSE_RESIDENCY_16_SAMPLES) ? VK_TRUE
                                                                   : VK_FALSE;
  deviceFeatures.sparseResidencyAliased =
      (featuresFlag & DeviceFeatures::SPARSE_RESIDENCY_ALIASED) ? VK_TRUE
                                                                : VK_FALSE;
  deviceFeatures.variableMultisampleRate =
      (featuresFlag & DeviceFeatures::VARIABLE_MULTISAMPLE_RATE) ? VK_TRUE
                                                                 : VK_FALSE;
  deviceFeatures.inheritedQueries =
      (featuresFlag & DeviceFeatures::INHERITED_QUERIES) ? VK_TRUE : VK_FALSE;

  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.queueCreateInfoCount = queueCreateInfos.size();
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount = requiredExtensions.size();
  createInfo.ppEnabledExtensionNames =
      (char const *const *)requiredExtensions.data();

  vec<str> layers = mInstance->getEnabledLayers();
  if (mInstance->isDebugMode()) {
    createInfo.enabledLayerCount = layers.size();
    createInfo.ppEnabledLayerNames = (char const *const *)layers.data();
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  if (vkCreateDevice(*physicalDevice, &createInfo, nullptr, &mHandle) !=
      VK_SUCCESS) {
    throw DeviceException("Failed to create logical device.");
  }
}

Device::~Device() {
  if (mHandle != VK_NULL_HANDLE) {
    vkDestroyDevice(mHandle, nullptr);
    mHandle = VK_NULL_HANDLE;
  }
}

shared<PhysicalDevice> const &Device::getPhysicalDevice() const {
  return mPhysicalDevice;
}

umap<QueueType, u32> const &Device::getQueueFamilyIndices() const {
  return mQueueFamilyIndices;
}

VkSampleCountFlagBits const &Device::getMaxSampleCount() const {
  return mPhysicalDevice->getMaxSampleCount();
}

vec<VkQueueFamilyProperties> const &Device::getQueueFamilies() const {
  return mPhysicalDevice->getQueueFamilies();
}

vec<VkExtensionProperties> const &Device::getSupportedExtensions() const {
  return mPhysicalDevice->getSupportedExtensions();
}

VkPhysicalDeviceProperties const &Device::getProperties() const {
  return mPhysicalDevice->getProperties();
}

VkPhysicalDeviceFeatures const &Device::getSupportedFeatures() const {
  return mPhysicalDevice->getSupportedFeatures();
}

VkPhysicalDeviceMemoryProperties const &Device::getMemoryProperties() const {
  return mPhysicalDevice->getMemoryProperties();
}

VkFormatProperties Device::getFormatProperties(VkFormat const &format) const {
  return mPhysicalDevice->getFormatProperties(format);
}

VkSurfaceCapabilitiesKHR Device::getSurfaceCapabilities() const {
  return mPhysicalDevice->getSurfaceCapabilities();
}

vec<VkSurfaceFormatKHR> Device::getSurfaceFormats() const {
  return mPhysicalDevice->getSurfaceFormats();
}

vec<VkPresentModeKHR> Device::getPresentModes() const {
  return mPhysicalDevice->getPresentModes();
}

u32 Device::findMemoryType(u32 typeFilter,
                           VkMemoryPropertyFlags properties) const {
  return mPhysicalDevice->findMemoryType(typeFilter, properties);
}

void Device::waitIdle() const { vkDeviceWaitIdle(mHandle); }

Device::operator VkDevice() const { return mHandle; }

shared<Device> Device::create(shared<Instance> instance,
                              shared<SDL::Window> window,
                              vec<DeviceFeatures> const &requiredFeatures,
                              uset<QueueType> const &requiredQueues,
                              vec<str> const &requiredExtensions) {
  shared<PhysicalDevice> physicalDevice = PhysicalDevice::pick(
      instance, window, requiredQueues, requiredExtensions);
  Device *device =
      new Device(instance, window, physicalDevice, requiredFeatures,
                 requiredQueues, requiredExtensions);
  return shared<Device>(device);
}

shared<Device> Device::create(shared<Instance> instance,
                              shared<SDL::Window> window,
                              shared<PhysicalDevice> physicalDevice,
                              vec<DeviceFeatures> const &requiredFeatures,
                              uset<QueueType> const &requiredQueues,
                              vec<str> const &requiredExtensions) {
  Device *device =
      new Device(instance, window, physicalDevice, requiredFeatures,
                 requiredQueues, requiredExtensions);
  return shared<Device>(device);
}

} // namespace Terreate::Vulkan
