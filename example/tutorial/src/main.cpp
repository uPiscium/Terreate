#include "../include/main.hpp"
#include "vulkan/vulkan_handles.hpp"

VkVertexInputBindingDescription Vertex::getBindingDescription() {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}

array<VkVertexInputAttributeDescription, 2> Vertex::getAttributeDescriptions() {
  array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, pos);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);

  return attributeDescriptions;
}

QueueFamilyIndices::operator uset<i32>() const {
  if (this->isComplete()) {
    return uset<i32>{graphicsFamily, presentFamily};
  } else {
    return uset<i32>{};
  }
}

void VulkanTutorial::initWindow() {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_JOYSTICK |
                SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC | SDL_INIT_CAMERA |
                SDL_INIT_SENSOR | SDL_INIT_AUDIO)) {
    throw std::runtime_error("Failed to initialize SDL.");
    return;
  }

  mWindow = SDL_CreateWindow("Vulkan Tutorial", WINDOW_WIDTH, WINDOW_HEIGHT,
                             SDL_WINDOW_VULKAN);
}

bool VulkanTutorial::checkValidationLayerSupport() {
  u32 layerCount;
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

  std::vector<VkLayerProperties> availableLayers(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  // std::cout << "Available Vulkan layers:\n";
  // for (const auto &layer : availableLayers) {
  //   std::cout << '\t' << layer.layerName << '\n';
  // }

  for (auto const &layer : VALIDATION_LAYERS) {
    bool layerFound = false;

    for (const auto &availableLayer : availableLayers) {
      if (std::strcmp(layer, availableLayer.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }

  return true;
}

vec<char const *> VulkanTutorial::getRequiredExtensions() {
  u32 extensionCount = 0;
  char const *const *extensions =
      SDL_Vulkan_GetInstanceExtensions(&extensionCount);
  if (!extensions) {
    throw std::runtime_error("Failed to get required Vulkan extensions.");
  }
  vec<char const *> requiredExtensions(extensions, extensions + extensionCount);

  if (mDebugMode) {
    requiredExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }

  return requiredExtensions;
}

void VulkanTutorial::populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
  createInfo = {};
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = debugCallback;
  createInfo.pUserData = nullptr; // Optional
}

void VulkanTutorial::createInstance(str const &appName, u32 const &appVersion) {
  if (mDebugMode && !this->checkValidationLayerSupport()) {
    throw std::runtime_error("Validation layers requested, but not available.");
  }

  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName.c_str();
  appInfo.applicationVersion = appVersion;
  appInfo.pEngineName = ENGINE_NAME;
  appInfo.engineVersion = ENGINE_VERSION;
  appInfo.apiVersion = VK_API_VERSION_1_4;

  vec<char const *> extensions = this->getRequiredExtensions();

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo = &appInfo;
  createInfo.enabledExtensionCount = extensions.size();
  createInfo.ppEnabledExtensionNames = extensions.data();

  VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
  if (mDebugMode) {
    createInfo.enabledLayerCount =
        sizeof(VALIDATION_LAYERS) / sizeof(VALIDATION_LAYERS[0]);
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;

    this->populateDebugMessengerCreateInfo(debugCreateInfo);
    createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
  } else {
    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = nullptr;
  }

  u32 extCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
  vec<VkExtensionProperties> availableExtensions(extCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extCount,
                                         availableExtensions.data());

  // std::cout << "Available Vulkan extensions:\n";
  // for (const auto &extension : availableExtensions) {
  //   std::cout << '\t' << extension.extensionName << '\n';
  // }

  if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create Vulkan instance.");
  }
}

VkResult VulkanTutorial::createDebugUtilsMessengerEXT(
    VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
    VkAllocationCallbacks const *pAllocator) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      mInstance, "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(mInstance, pCreateInfo, nullptr, &mDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

VkResult VulkanTutorial::destroyDebugUtilsMessengerEXT(
    VkAllocationCallbacks const *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      mInstance, "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr && mDebugMessenger != VK_NULL_HANDLE) {
    func(mInstance, mDebugMessenger, nullptr);
    mDebugMessenger = VK_NULL_HANDLE;
    return VK_SUCCESS;
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void VulkanTutorial::setupDebugMessenger() {
  if (!mDebugMode) {
    return;
  }

  VkDebugUtilsMessengerCreateInfoEXT createInfo{};
  this->populateDebugMessengerCreateInfo(createInfo);

  if (this->createDebugUtilsMessengerEXT(&createInfo, nullptr) != VK_SUCCESS) {
    throw std::runtime_error("Failed to set up debug messenger.");
  }
}

void VulkanTutorial::createSurface() {
  if (!SDL_Vulkan_CreateSurface(mWindow, mInstance, nullptr, &mSurface)) {
    throw std::runtime_error("Failed to create window surface.");
  }
}

QueueFamilyIndices VulkanTutorial::findQueueFamilies(VkPhysicalDevice device) {
  QueueFamilyIndices indices;

  u32 queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
  vec<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  u32 index = 0;
  for (auto const &queueFamily : queueFamilies) {
    if (queueFamily.queueCount > 0 &&
        queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = index;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, 0, mSurface, &presentSupport);
    if (presentSupport) {
      indices.presentFamily = index;
    }

    if (indices.isComplete()) {
      break;
    }

    ++index;
  }

  return indices;
}

bool VulkanTutorial::checkDeviceExtensionSupport(VkPhysicalDevice device) {
  u32 extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);

  vec<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  u32 length = sizeof(DEVICE_EXTENSIONS) / sizeof(DEVICE_EXTENSIONS[0]);
  uset<str> requiredExtensions(DEVICE_EXTENSIONS, DEVICE_EXTENSIONS + length);

  for (auto const &extension : availableExtensions) {
    requiredExtensions.erase(extension.extensionName);
  }

  return requiredExtensions.empty();
}

SwapChainSupportDetails
VulkanTutorial::querySwapChainSupport(VkPhysicalDevice device) {
  SwapChainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface,
                                            &details.capabilities);

  u32 formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount,
                                         details.formats.data());
  }

  u32 presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, mSurface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

int VulkanTutorial::rateDevice(VkPhysicalDevice device) {
  if (!this->findQueueFamilies(device).isComplete()) {
    return -1;
  }

  if (!this->checkDeviceExtensionSupport(device)) {
    return -1;
  }

  SwapChainSupportDetails swapChainSupport =
      this->querySwapChainSupport(device);
  if (swapChainSupport.formats.empty() ||
      swapChainSupport.presentModes.empty()) {
    return -1;
  }

  int score = 0;
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  // Discrete GPUs have a significant performance advantage
  if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }

  return score;
}

void VulkanTutorial::pickPhysicalDevice() {
  u32 deviceCount = 0;
  vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("Failed to find GPUs with Vulkan support.");
  }

  vec<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

  // std::cout << "Available Vulkan devices:\n";
  // for (auto const &device : devices) {
  //   VkPhysicalDeviceProperties deviceProperties;
  //   vkGetPhysicalDeviceProperties(device, &deviceProperties);
  //   std::cout << '\t' << deviceProperties.deviceName << '\n';
  // }

  VkPhysicalDevice best = VK_NULL_HANDLE;
  int bestScore = -1;
  for (auto const &device : devices) {
    int score = this->rateDevice(device);
    if (score > bestScore) {
      best = device;
      bestScore = score;
    }
  }

  if (bestScore == -1 || best == VK_NULL_HANDLE) {
    throw std::runtime_error("Failed to find a suitable GPU.");
  }
  mPhysicalDevice = best;
}

void VulkanTutorial::createLogicalDevice() {
  QueueFamilyIndices indices = this->findQueueFamilies(mPhysicalDevice);

  uset<i32> uniqueQueueFamilies = indices;

  vec<VkDeviceQueueCreateInfo> queueCreateInfos;
  float queuePriority = 1.0f;
  for (i32 queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = queueFamily;
    queueCreateInfo.queueCount = 1;
    queueCreateInfo.pQueuePriorities = &queuePriority;
    queueCreateInfos.push_back(queueCreateInfo);
  }

  VkPhysicalDeviceFeatures deviceFeatures{};
  VkDeviceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.queueCreateInfoCount = queueCreateInfos.size();
  createInfo.pEnabledFeatures = &deviceFeatures;

  createInfo.enabledExtensionCount =
      sizeof(DEVICE_EXTENSIONS) / sizeof(DEVICE_EXTENSIONS[0]);
  createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS;

  if (mDebugMode) {
    createInfo.enabledLayerCount =
        sizeof(VALIDATION_LAYERS) / sizeof(VALIDATION_LAYERS[0]);
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
  } else {
    createInfo.enabledLayerCount = 0;
  }

  if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create logical device.");
  }
}

void VulkanTutorial::createQueue() {
  QueueFamilyIndices indices = this->findQueueFamilies(mPhysicalDevice);
  vkGetDeviceQueue(mDevice, indices.graphicsFamily, 0, &mGraphicsQueue);
  vkGetDeviceQueue(mDevice, indices.presentFamily, 0, &mPresentQueue);
}

VkSurfaceFormatKHR VulkanTutorial::chooseSwapSurfaceFormat(
    vec<VkSurfaceFormatKHR> const &availableFormats) {
  for (auto const &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR VulkanTutorial::chooseSwapPresentMode(
    vec<VkPresentModeKHR> const &availablePresentModes) {
  for (auto const &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
VulkanTutorial::chooseSwapExtent(VkSurfaceCapabilitiesKHR const &capabilities) {
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    SDL_GetWindowSizeInPixels(mWindow, &width, &height);
    VkExtent2D actualExtent = {static_cast<u32>(width),
                               static_cast<u32>(height)};
    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);
    return actualExtent;
  }
}

void VulkanTutorial::createSwapchain() {
  SwapChainSupportDetails swapChainSupport =
      this->querySwapChainSupport(mPhysicalDevice);

  VkSurfaceFormatKHR surfaceFormat =
      this->chooseSwapSurfaceFormat(swapChainSupport.formats);
  VkPresentModeKHR presentMode =
      this->chooseSwapPresentMode(swapChainSupport.presentModes);
  VkExtent2D extent = this->chooseSwapExtent(swapChainSupport.capabilities);
  u32 imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = mSurface;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = this->findQueueFamilies(mPhysicalDevice);
  u32 queueFamilyIndices[] = {static_cast<u32>(indices.graphicsFamily),
                              static_cast<u32>(indices.presentFamily)};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  }

  createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(mDevice, &createInfo, nullptr, &mSwapchain) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create swap chain.");
  }

  vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount, nullptr);
  mSwapchainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(mDevice, mSwapchain, &imageCount,
                          mSwapchainImages.data());

  mSwapchainImageFormat = surfaceFormat.format;
  mSwapchainExtent = extent;
}

void VulkanTutorial::createImageViews() {
  mSwapchainImageViews.resize(mSwapchainImages.size());

  for (u32 i = 0; i < mSwapchainImages.size(); ++i) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = mSwapchainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = mSwapchainImageFormat;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(mDevice, &createInfo, nullptr,
                          &mSwapchainImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create image view.");
    }
  }
}

VkShaderModule VulkanTutorial::createShaderModule(vec<char> const &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<u32 const *>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(mDevice, &createInfo, nullptr, &shaderModule) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module.");
  }

  return shaderModule;
}

void VulkanTutorial::createRenderPass() {
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = mSwapchainImageFormat;
  colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if (vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create RenderPass.");
  }
}

void VulkanTutorial::createDescriptorSetLayout() {
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = 1;
  layoutInfo.pBindings = &uboLayoutBinding;

  if (vkCreateDescriptorSetLayout(mDevice, &layoutInfo, nullptr,
                                  &mDescriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout.");
  }
}

void VulkanTutorial::createGraphicsPipeline() {
  vec<char> vert = this->readFile("resource/shader/shader.vert.spv");
  vec<char> frag = this->readFile("resource/shader/shader.frag.spv");

  VkShaderModule vertShaderModule = this->createShaderModule(vert);
  VkShaderModule fragShaderModule = this->createShaderModule(frag);

  VkPipelineShaderStageCreateInfo vertInfo{};
  vertInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertInfo.module = vertShaderModule;
  vertInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragInfo{};
  fragInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragInfo.module = fragShaderModule;
  fragInfo.pName = "main";

  VkPipelineShaderStageCreateInfo stages[] = {vertInfo, fragInfo};

  vec<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                       VK_DYNAMIC_STATE_SCISSOR};
  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = static_cast<u32>(dynamicStates.size());
  dynamicState.pDynamicStates = dynamicStates.data();

  auto bindingDescription = Vertex::getBindingDescription();
  auto attributeDescriptions = Vertex::getAttributeDescriptions();

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 1;
  vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
  vertexInputInfo.vertexAttributeDescriptionCount =
      attributeDescriptions.size();
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  // VkViewport viewport{};
  // viewport.x = 0.0f;
  // viewport.y = 0.0f;
  // viewport.width = (float)mSwapchainExtent.width;
  // viewport.height = (float)mSwapchainExtent.height;
  // viewport.minDepth = 0.0f;
  // viewport.maxDepth = 1.0f;

  // VkRect2D scissor{};
  // scissor.offset = {0, 0};
  // scissor.extent = mSwapchainExtent;

  VkPipelineViewportStateCreateInfo viewportState{};
  viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewportState.viewportCount = 1;
  viewportState.scissorCount = 1;

  VkPipelineRasterizationStateCreateInfo rasterizer{};
  rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rasterizer.depthClampEnable = VK_FALSE;
  rasterizer.rasterizerDiscardEnable = VK_FALSE;
  rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
  rasterizer.lineWidth = 1.0f;
  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f;
  rasterizer.depthBiasClamp = 0.0f;
  rasterizer.depthBiasSlopeFactor = 0.0f;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;
  multisampling.pSampleMask = nullptr;
  multisampling.alphaToCoverageEnable = VK_FALSE;
  multisampling.alphaToOneEnable = VK_FALSE;

  VkPipelineColorBlendAttachmentState colorBlendAttachment{};
  colorBlendAttachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  colorBlendAttachment.blendEnable = VK_TRUE;
  colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  colorBlendAttachment.dstColorBlendFactor =
      VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
  colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo colorBlending{};
  colorBlending.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  colorBlending.logicOpEnable = VK_FALSE;
  colorBlending.logicOp = VK_LOGIC_OP_COPY;
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f;
  colorBlending.blendConstants[1] = 0.0f;
  colorBlending.blendConstants[2] = 0.0f;
  colorBlending.blendConstants[3] = 0.0f;

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &mDescriptorSetLayout;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(mDevice, &pipelineLayoutInfo, nullptr,
                             &mPipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout.");
  }

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = 2;
  pipelineInfo.pStages = stages;
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = nullptr;
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = mPipelineLayout;
  pipelineInfo.renderPass = mRenderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
  pipelineInfo.basePipelineIndex = -1;

  if (vkCreateGraphicsPipelines(mDevice, nullptr, 1, &pipelineInfo, nullptr,
                                &mGraphicsPipeline) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipeline.");
  }

  vkDestroyShaderModule(mDevice, fragShaderModule, nullptr);
  vkDestroyShaderModule(mDevice, vertShaderModule, nullptr);
}

void VulkanTutorial::createFramebuffers() {
  mSwapchainFramebuffers.resize(mSwapchainImageViews.size());

  for (u32 i = 0; i < mSwapchainImageViews.size(); ++i) {
    VkImageView attachments[] = {mSwapchainImageViews[i]};
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = mRenderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = mSwapchainExtent.width;
    framebufferInfo.height = mSwapchainExtent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(mDevice, &framebufferInfo, nullptr,
                            &mSwapchainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create framebuffer.");
    }
  }
}

void VulkanTutorial::createCommandPool() {
  QueueFamilyIndices queueFamilyIndices =
      this->findQueueFamilies(mPhysicalDevice);

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;

  if (vkCreateCommandPool(mDevice, &poolInfo, nullptr, &mCommandPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create command pool.");
  }
}

u32 VulkanTutorial::findMemoryType(u32 typeFilter,
                                   VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(mPhysicalDevice, &memProperties);

  for (u32 i = 0; i < memProperties.memoryTypeCount; ++i) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }

  throw std::runtime_error("Failed to find suitable memory type.");
}

void VulkanTutorial::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                                  VkMemoryPropertyFlags properties,
                                  VkBuffer &buffer,
                                  VkDeviceMemory &bufferMemory) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bufferInfo.flags = 0; // Optional

  if (vkCreateBuffer(mDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create vertex buffer.");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(mDevice, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      this->findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(mDevice, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate vertex buffer memory.");
  }

  vkBindBufferMemory(mDevice, buffer, bufferMemory, 0);
}

void VulkanTutorial::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
                                VkDeviceSize size) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = mCommandPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(mDevice, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(mGraphicsQueue, 1, &submitInfo,
                VK_NULL_HANDLE); // TODO: change to use fence
  vkQueueWaitIdle(mGraphicsQueue);
  vkFreeCommandBuffers(mDevice, mCommandPool, 1, &commandBuffer);
}

void VulkanTutorial::createVertexBuffer() {
  VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  this->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(mDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
  std::memcpy(data, vertices.data(), (size_t)bufferSize);
  vkUnmapMemory(mDevice, stagingBufferMemory);

  this->createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mVertexBuffer, mVertexBufferMemory);
  this->copyBuffer(stagingBuffer, mVertexBuffer, bufferSize);

  vkDestroyBuffer(mDevice, stagingBuffer, nullptr);
  vkFreeMemory(mDevice, stagingBufferMemory, nullptr);
}

void VulkanTutorial::createIndexBuffer() {
  VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  this->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(mDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
  std::memcpy(data, indices.data(), (size_t)bufferSize);
  vkUnmapMemory(mDevice, stagingBufferMemory);

  this->createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mIndexBuffer, mIndexBufferMemory);
  this->copyBuffer(stagingBuffer, mIndexBuffer, bufferSize);

  vkDestroyBuffer(mDevice, stagingBuffer, nullptr);
  vkFreeMemory(mDevice, stagingBufferMemory, nullptr);
}

void VulkanTutorial::createUniformBuffers() {
  VkDeviceSize bufferSize = sizeof(UniformBufferObject);

  mUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
  mUniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
  mUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    this->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       mUniformBuffers[i], mUniformBuffersMemory[i]);
    vkMapMemory(mDevice, mUniformBuffersMemory[i], 0, bufferSize, 0,
                &mUniformBuffersMapped[i]);
  }
}

void VulkanTutorial::createDescriptorPool() {
  VkDescriptorPoolSize poolSize{};
  poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSize.descriptorCount = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = 1;
  poolInfo.pPoolSizes = &poolSize;
  poolInfo.maxSets = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);

  if (vkCreateDescriptorPool(mDevice, &poolInfo, nullptr, &mDescriptorPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor pool.");
  }
}

void VulkanTutorial::createDescriptorSets() {
  vec<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT,
                                     mDescriptorSetLayout);
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = mDescriptorPool;
  allocInfo.descriptorSetCount = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);
  allocInfo.pSetLayouts = layouts.data();

  mDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
  if (vkAllocateDescriptorSets(mDevice, &allocInfo, mDescriptorSets.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate descriptor sets.");
  }

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = mUniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = mDescriptorSets[i];
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr;       // Optional
    descriptorWrite.pTexelBufferView = nullptr; // Optional

    vkUpdateDescriptorSets(mDevice, 1, &descriptorWrite, 0, nullptr);
  }
}

void VulkanTutorial::createCommandBuffers() {
  mCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = mCommandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = mCommandBuffers.size();

  if (vkAllocateCommandBuffers(mDevice, &allocInfo, mCommandBuffers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers.");
  }
}

void VulkanTutorial::recordCommandBuffer(VkCommandBuffer commandBuffer,
                                         u32 imageIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                  // Optional
  beginInfo.pInheritanceInfo = nullptr; // Optional

  if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin recording.");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = mRenderPass;
  renderPassInfo.framebuffer = mSwapchainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = mSwapchainExtent;

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(commandBuffer, &renderPassInfo,
                       VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    mGraphicsPipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)mSwapchainExtent.width;
  viewport.height = (float)mSwapchainExtent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = mSwapchainExtent;
  vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

  VkBuffer vertexBuffers[] = {mVertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
  vkCmdBindIndexBuffer(commandBuffer, mIndexBuffer, 0, VK_INDEX_TYPE_UINT16);

  vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                          mPipelineLayout, 0, 1,
                          &mDescriptorSets[mCurrentFrame], 0, nullptr);
  vkCmdDrawIndexed(commandBuffer, static_cast<u32>(indices.size()), 1, 0, 0, 0);

  vkCmdEndRenderPass(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer.");
  }
}

void VulkanTutorial::createSyncObjects() {
  mImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  mRenderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  mInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    if (vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr,
                          &mImageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr,
                          &mRenderFinishedSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(mDevice, &fenceInfo, nullptr, &mInFlightFences[i]) !=
            VK_SUCCESS) {
      throw std::runtime_error("Failed to create synchronization objects.");
    }
  }
}

void VulkanTutorial::cleanupSwapchain() {
  for (u32 i = 0; i < mSwapchainFramebuffers.size(); ++i) {
    vkDestroyFramebuffer(mDevice, mSwapchainFramebuffers[i], nullptr);
  }
  for (u32 i = 0; i < mSwapchainImageViews.size(); ++i) {
    vkDestroyImageView(mDevice, mSwapchainImageViews[i], nullptr);
  }
  vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
}

void VulkanTutorial::recreateSwapchain() {
  int width = 0, height = 0;
  SDL_GetWindowSizeInPixels(mWindow, &width, &height);
  while (width == 0 || height == 0) {
    SDL_GetWindowSizeInPixels(mWindow, &width, &height);
    SDL_WaitEvent(nullptr);
  }
  vkDeviceWaitIdle(mDevice);

  this->cleanupSwapchain();
  this->createSwapchain();
  this->createImageViews();
  this->createFramebuffers();
}

void VulkanTutorial::initVulkan() {
  this->createInstance("Vulkan Tutorial", VK_MAKE_VERSION(0, 1, 0));
  this->setupDebugMessenger();
  this->createSurface();
  this->pickPhysicalDevice();
  this->createLogicalDevice();
  this->createQueue();
  this->createSwapchain();
  this->createImageViews();
  this->createRenderPass();
  this->createDescriptorSetLayout();
  this->createGraphicsPipeline();
  this->createFramebuffers();
  this->createCommandPool();
  this->createVertexBuffer();
  this->createIndexBuffer();
  this->createUniformBuffers();
  this->createDescriptorPool();
  this->createDescriptorSets();
  this->createCommandBuffers();
  this->createSyncObjects();
}

bool VulkanTutorial::pollEvents() {
  SDL_Event handle;
  while (SDL_PollEvent(&handle)) {
    switch (handle.type) {
    case SDL_EVENT_QUIT:
      return false;

    case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    case SDL_EVENT_WINDOW_DESTROYED:
      return false;

    case SDL_EVENT_WINDOW_RESIZED:
    case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
      mFramebufferResized = true;

    default:
      break;
    }
  }

  return true;
}

void VulkanTutorial::updateUniformBuffer(u32 currentImage) {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  UniformBufferObject ubo{};
  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
                          glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.view =
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj = glm::perspective(
      glm::radians(45.0f),
      mSwapchainExtent.width / (float)mSwapchainExtent.height, 0.1f, 10.0f);
  ubo.proj[1][1] *= -1;
  std::memcpy(mUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void VulkanTutorial::drawFrame() {
  vkWaitForFences(mDevice, 1, &mInFlightFences[mCurrentFrame], VK_TRUE,
                  UINT64_MAX);

  u32 imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      mDevice, mSwapchain, UINT64_MAX, mImageAvailableSemaphores[mCurrentFrame],
      VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    this->recreateSwapchain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire swap chain image.");
  }

  vkResetFences(mDevice, 1, &mInFlightFences[mCurrentFrame]);

  vkResetCommandBuffer(mCommandBuffers[mCurrentFrame], 0);
  this->recordCommandBuffer(mCommandBuffers[mCurrentFrame], imageIndex);

  this->updateUniformBuffer(mCurrentFrame);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  VkSemaphore waitSemaphores[] = {mImageAvailableSemaphores[mCurrentFrame]};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &mCommandBuffers[mCurrentFrame];

  VkSemaphore signalSemaphores[] = {mRenderFinishedSemaphores[mCurrentFrame]};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(mGraphicsQueue, 1, &submitInfo,
                    mInFlightFences[mCurrentFrame]) != VK_SUCCESS) {
    throw std::runtime_error("Failed to submit draw command buffer.");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapChains[] = {mSwapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapChains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr;

  result = vkQueuePresentKHR(mPresentQueue, &presentInfo);

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      mFramebufferResized) {
    mFramebufferResized = false;
    this->recreateSwapchain();
  } else if (result != VK_SUCCESS) {
    throw std::runtime_error("Failed to present swap chain image.");
  }

  mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanTutorial::mainLoop() {
  bool loop = true;
  while (loop) {
    loop = this->pollEvents();
    this->drawFrame();
  }

  vkDeviceWaitIdle(mDevice);
}

void VulkanTutorial::cleanup() {
  this->cleanupSwapchain();

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    vkUnmapMemory(mDevice, mUniformBuffersMemory[i]);
    vkDestroyBuffer(mDevice, mUniformBuffers[i], nullptr);
    vkFreeMemory(mDevice, mUniformBuffersMemory[i], nullptr);
  }

  vkDestroyDescriptorPool(mDevice, mDescriptorPool, nullptr);

  vkDestroyDescriptorSetLayout(mDevice, mDescriptorSetLayout, nullptr);

  vkDestroyBuffer(mDevice, mVertexBuffer, nullptr);
  vkFreeMemory(mDevice, mVertexBufferMemory, nullptr);

  vkDestroyBuffer(mDevice, mIndexBuffer, nullptr);
  vkFreeMemory(mDevice, mIndexBufferMemory, nullptr);

  if (mGraphicsPipeline != VK_NULL_HANDLE) {
    vkDestroyPipeline(mDevice, mGraphicsPipeline, nullptr);
    mGraphicsPipeline = VK_NULL_HANDLE;
  }

  if (mPipelineLayout != VK_NULL_HANDLE) {
    vkDestroyPipelineLayout(mDevice, mPipelineLayout, nullptr);
    mPipelineLayout = VK_NULL_HANDLE;
  }

  if (mRenderPass != VK_NULL_HANDLE) {
    vkDestroyRenderPass(mDevice, mRenderPass, nullptr);
    mRenderPass = VK_NULL_HANDLE;
  }

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    if (mInFlightFences[i] != VK_NULL_HANDLE) {
      vkDestroyFence(mDevice, mInFlightFences[i], nullptr);
      mInFlightFences[i] = VK_NULL_HANDLE;
    }

    if (mRenderFinishedSemaphores[i] != VK_NULL_HANDLE) {
      vkDestroySemaphore(mDevice, mRenderFinishedSemaphores[i], nullptr);
      mRenderFinishedSemaphores[i] = VK_NULL_HANDLE;
    }

    if (mImageAvailableSemaphores[i] != VK_NULL_HANDLE) {
      vkDestroySemaphore(mDevice, mImageAvailableSemaphores[i], nullptr);
      mImageAvailableSemaphores[i] = VK_NULL_HANDLE;
    }
  }

  if (mCommandPool != VK_NULL_HANDLE) {
    vkDestroyCommandPool(mDevice, mCommandPool, nullptr);
    mCommandPool = VK_NULL_HANDLE;
  }

  vkDestroyDevice(mDevice, nullptr);

  if (mDebugMode) {
    this->destroyDebugUtilsMessengerEXT(nullptr);
  }

  if (mSurface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
    mSurface = VK_NULL_HANDLE;
  }

  if (mInstance != VK_NULL_HANDLE) {
    vkDestroyInstance(mInstance, nullptr);
    mInstance = VK_NULL_HANDLE;
  }

  if (mWindow) {
    SDL_DestroyWindow(mWindow);
    mWindow = nullptr;
  }

  SDL_Quit();
}

VKAPI_ATTR VkBool32 VKAPI_CALL VulkanTutorial::debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
    void *pUserData) {
  switch (messageSeverity) {
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
    return VK_FALSE;
    std::cerr << "VERBOSE: ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
    return VK_FALSE;
    std::cerr << "INFO: ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
    return VK_FALSE;
    std::cerr << "WARNING: ";
    break;
  case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
    std::cerr << "ERROR: ";
    break;
  default:
    std::cerr << "UNKNOWN: ";
    break;
  }
  std::cerr << pCallbackData->pMessage << std::endl;
  return VK_FALSE;
}

vec<char> VulkanTutorial::readFile(str const &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file.");
  }

  size_t fsize = (size_t)file.tellg();
  vec<char> buffer(fsize);
  file.seekg(0);
  file.read(buffer.data(), fsize);
  file.close();

  return buffer;
}

void VulkanTutorial::run() {
  this->initWindow();
  this->initVulkan();
  this->mainLoop();
  this->cleanup();
}

int main() {
  VulkanTutorial app(true);

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
