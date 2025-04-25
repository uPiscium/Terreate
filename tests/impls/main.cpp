#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

#include <main.hpp>

std::vector<char> VulkanTriangle::readFile(std::string const &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file: " + filename);
  }

  size_t fileSize = (size_t)file.tellg();
  std::vector<char> buffer(fileSize);
  file.seekg(0);
  file.read(buffer.data(), fileSize);
  file.close();
  return buffer;
}

void VulkanTriangle::initWindow() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  mWindow =
      glfwCreateWindow(WIDTH, HEIGHT, "Vulkan Triangle", nullptr, nullptr);
}

void VulkanTriangle::loadDebugUtilsMessengerEXT() {
  trCreateDebugUtilsMessengerEXT =
      (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          mInstance, "vkCreateDebugUtilsMessengerEXT");
  trDestroyDebugUtilsMessengerEXT =
      (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
          mInstance, "vkDestroyDebugUtilsMessengerEXT");

  if (!trCreateDebugUtilsMessengerEXT || !trDestroyDebugUtilsMessengerEXT) {
    throw std::runtime_error("Could not load vkCreateDebugUtilsMessengerEXT or "
                             "vkDestroyDebugUtilsMessengerEXT!");
  }
}

VkApplicationInfo VulkanTriangle::createAppInfo() {
  VkApplicationInfo appInfo{};
  appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = "Vulkan Triangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName = "No Engine";
  appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion = VK_API_VERSION_1_4;
  return appInfo;
}

void VulkanTriangle::createInstance() {
  mDebugger = new TestDebugger();

  VkInstanceCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  auto appInfo = this->createAppInfo();
  createInfo.pApplicationInfo = &appInfo;

  if (Terreate::API::checkValidationLayerSupport()) {
    createInfo.enabledLayerCount =
        static_cast<uint32_t>(VALIDATION_LAYERS.size());
    createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
    createInfo.pNext = mDebugger->getCreateInfo();
  }

  auto exts = Terreate::API::getRequiredExts();
  createInfo.enabledExtensionCount = static_cast<uint32_t>(exts.size());
  createInfo.ppEnabledExtensionNames = exts.data();

  if (vkCreateInstance(&createInfo, nullptr, &mInstance) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create instance!");
  }

  // Terreate::API::loadEXTfunctions(mInstance);

  mInstance =
      Terreate::API::createInstance("VulkanTriangle", {1, 0, 0}, mDebugger);
}

bool VulkanTriangle::isCompleteQueueFamily(
    QueueFamilyIndices const &queueFamily) {
  return queueFamily.graphicsFamily.has_value() &&
         queueFamily.presentFamily.has_value();
}

QueueFamilyIndices VulkanTriangle::findQueueFamilies(VkPhysicalDevice device) {
  QueueFamilyIndices indices;

  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
  std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
                                           queueFamilies.data());

  uint32_t i = 0;
  for (auto const &queueFamily : queueFamilies) {
    if (queueFamily.queueCount > 0 &&
        (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
      indices.graphicsFamily = i;
    }

    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mSurface, &presentSupport);
    if (presentSupport) {
      indices.presentFamily = i;
    }

    if (this->isCompleteQueueFamily(indices)) {
      break;
    }

    ++i;
  }
  return indices;
}

int VulkanTriangle::rateDeviceSuitability(VkPhysicalDevice device) {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures deviceFeatures;
  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);
  int score = 0;

  switch (deviceProperties.deviceType) {
  case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
    score += DISCRETE_GPU_SCORE;
    break;
  case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
    score += INTEGRATED_GPU_SCORE;
    break;
  case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
    score += VIRTUAL_GPU_SCORE;
    break;
  case VK_PHYSICAL_DEVICE_TYPE_CPU:
    break;
  // case VK_PHYSICAL_DEVICE_TYPE_OTHER:
  //   return 0;
  default:
    return 0;
  }

  QueueFamilyIndices indices = this->findQueueFamilies(device);
  if (!this->isCompleteQueueFamily(indices)) {
    return 0;
  }

  score += deviceProperties.limits.maxImageDimension2D;

  return score;
}

bool VulkanTriangle::checkDeviceExtensionSupport(VkPhysicalDevice device) {
  uint32_t extensionCount;
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       nullptr);
  std::vector<VkExtensionProperties> availableExtensions(extensionCount);
  vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
                                       availableExtensions.data());

  std::set<std::string> requiredExtensions(DEVICE_EXTENSIONS.begin(),
                                           DEVICE_EXTENSIONS.end());

  for (auto const &exts : availableExtensions) {
    requiredExtensions.erase(exts.extensionName);
  }

  return requiredExtensions.empty();
}

SwapchainSupportDetails
VulkanTriangle::querySwapchainSupport(VkPhysicalDevice device) {
  SwapchainSupportDetails details;
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mSurface,
                                            &details.capabilities);
  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount, nullptr);
  if (formatCount != 0) {
    details.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, mSurface, &formatCount,
                                         details.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, mSurface, &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    details.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        device, mSurface, &presentModeCount, details.presentModes.data());
  }

  return details;
}

void VulkanTriangle::pickPhysicalDevice() {
  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(mInstance, &deviceCount, nullptr);
  if (deviceCount == 0) {
    throw std::runtime_error("Failed to find GPUs with Vulkan support!");
  }
  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(mInstance, &deviceCount, devices.data());

  for (auto &device : devices) {
    if (!this->checkDeviceExtensionSupport(device)) {
      continue;
    }

    auto score = this->rateDeviceSuitability(device);
    if (score <= 0) {
      continue;
    }

    SwapchainSupportDetails swapchainSupport =
        this->querySwapchainSupport(device);
    if (swapchainSupport.formats.empty() ||
        swapchainSupport.presentModes.empty()) {
      continue;
    }

    mDevices.insert(std::make_pair(score, device));
  }

  if (mDevices.empty()) {
    throw std::runtime_error("Failed to find a suitable GPU!");
  }

  mPhysicalDevice = mDevices.rbegin()->second;
}

void VulkanTriangle::createLogicalDevice() {
  QueueFamilyIndices indices = this->findQueueFamilies(mPhysicalDevice);

  std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
  std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(),
                                            indices.presentFamily.value()};

  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
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
  createInfo.queueCreateInfoCount =
      static_cast<uint32_t>(queueCreateInfos.size());
  createInfo.pQueueCreateInfos = queueCreateInfos.data();
  createInfo.pEnabledFeatures = &deviceFeatures;
  createInfo.enabledExtensionCount =
      static_cast<uint32_t>(DEVICE_EXTENSIONS.size());
  createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data();

#ifdef TERREATE_DEBUG_BUILD
  createInfo.enabledLayerCount =
      static_cast<uint32_t>(VALIDATION_LAYERS.size());
  createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
#else
  createInfo.enabledLayerCount = 0;
#endif

  if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create logical device!");
  }

  vkGetDeviceQueue(mDevice, indices.graphicsFamily.value(), 0, &mGraphicsQueue);
  vkGetDeviceQueue(mDevice, indices.presentFamily.value(), 0, &mPresentQueue);
}

void VulkanTriangle::createSurface() {
  if (glfwCreateWindowSurface(mInstance, mWindow, nullptr, &mSurface) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create window surface!");
  }
}

VkSurfaceFormatKHR VulkanTriangle::pickSurfaceFormat(
    std::vector<VkSurfaceFormatKHR> const &availableFormats) {
  for (auto const &format : availableFormats) {
    if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
        format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return format;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR VulkanTriangle::pickPresentMode(
    std::vector<VkPresentModeKHR> const &availablePresentModes) {
  for (auto const &presentMode : availablePresentModes) {
    if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return presentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D
VulkanTriangle::pickExtent(VkSurfaceCapabilitiesKHR const &capabilities) {
  if (capabilities.currentExtent.width !=
      std::numeric_limits<uint32_t>::max()) {
    return capabilities.currentExtent;
  } else {
    int width, height;
    glfwGetFramebufferSize(mWindow, &width, &height);
    VkExtent2D extent = {static_cast<uint32_t>(width),
                         static_cast<uint32_t>(height)};
    extent.width = std::clamp(extent.width, capabilities.minImageExtent.width,
                              capabilities.maxImageExtent.width);
    extent.height =
        std::clamp(extent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);
    return extent;
  }
}

void VulkanTriangle::createSwapchain() {
  SwapchainSupportDetails details =
      this->querySwapchainSupport(mPhysicalDevice);
  VkSurfaceFormatKHR surfaceFormat = this->pickSurfaceFormat(details.formats);
  VkPresentModeKHR presentMode = this->pickPresentMode(details.presentModes);
  VkExtent2D extent = this->pickExtent(details.capabilities);

  uint32_t const &minImgCount = details.capabilities.minImageCount;
  uint32_t const &maxImgCount = details.capabilities.maxImageCount;
  uint32_t imgCount = minImgCount + 1;
  imgCount =
      ((maxImgCount > 0) && (imgCount > maxImgCount)) ? maxImgCount : imgCount;

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = mSurface;
  createInfo.minImageCount = imgCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  QueueFamilyIndices indices = this->findQueueFamilies(mPhysicalDevice);
  uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(),
                                   indices.presentFamily.value()};

  if (indices.graphicsFamily != indices.presentFamily) {
    createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;     // optional
    createInfo.pQueueFamilyIndices = nullptr; // optional
  }

  createInfo.preTransform = details.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(mDevice, &createInfo, nullptr, &mSwapchain) !=
      VK_SUCCESS) {
    std::runtime_error("Failed to create swapchain.");
  }

  uint32_t swapchainImgCount;
  vkGetSwapchainImagesKHR(mDevice, mSwapchain, &swapchainImgCount, nullptr);
  mSwapchainImages.resize(swapchainImgCount);
  vkGetSwapchainImagesKHR(mDevice, mSwapchain, &swapchainImgCount,
                          mSwapchainImages.data());

  mSwapchainProperty.imageFormat = surfaceFormat.format;
  mSwapchainProperty.extent = extent;
}

void VulkanTriangle::createImageViews() {
  mSwapchainImageViews.resize(mSwapchainImages.size());
  for (size_t i = 0; i < mSwapchainImages.size(); ++i) {
    VkImageViewCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = mSwapchainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = mSwapchainProperty.imageFormat;

    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

    createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel = 0;
    createInfo.subresourceRange.levelCount = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount = 1;

    createInfo.pNext = nullptr;

    if (vkCreateImageView(mDevice, &createInfo, nullptr,
                          &mSwapchainImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create image view.");
    }
  }
}

VkShaderModule
VulkanTriangle::createShaderModule(std::vector<char> const &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<uint32_t const *>(code.data());

  VkShaderModule module;
  if (vkCreateShaderModule(mDevice, &createInfo, nullptr, &module) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module.");
  }

  return module;
}

void VulkanTriangle::createRenderPass() {
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = mSwapchainProperty.imageFormat;
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

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &colorAttachment;
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if (vkCreateRenderPass(mDevice, &renderPassInfo, nullptr, &mRenderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create render pass.");
  }
}

void VulkanTriangle::createGraphicsPipeline() {
  auto vertShaderCode = readFile("tests/resources/shader/vert.spv");
  auto fragShaderCode = readFile("tests/resources/shader/frag.spv");

  VkShaderModule vertModule = this->createShaderModule(vertShaderCode);
  VkShaderModule fragModule = this->createShaderModule(fragShaderCode);

  VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vertShaderStageInfo.module = vertModule;
  vertShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
  fragShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  fragShaderStageInfo.module = fragModule;
  fragShaderStageInfo.pName = "main";

  VkPipelineShaderStageCreateInfo stages[] = {vertShaderStageInfo,
                                              fragShaderStageInfo};

  VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
  vertexInputInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertexInputInfo.vertexBindingDescriptionCount = 0;
  vertexInputInfo.pVertexBindingDescriptions = nullptr;
  vertexInputInfo.vertexAttributeDescriptionCount = 0;
  vertexInputInfo.pVertexAttributeDescriptions = nullptr;

  VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
  inputAssembly.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  inputAssembly.primitiveRestartEnable = VK_FALSE;

  // VkViewport viewport{};
  // viewport.x = 0;
  // viewport.y = 0;
  // viewport.width = (float)mSwapchainProperty.extent.width;
  // viewport.height = (float)mSwapchainProperty.extent.height;
  // viewport.minDepth = 0.0f;
  // viewport.maxDepth = 1.0f;

  // VkRect2D scissor{};
  // scissor.offset = {0, 0};
  // scissor.extent = mSwapchainProperty.extent;

  std::vector<VkDynamicState> dynamicStates = {VK_DYNAMIC_STATE_VIEWPORT,
                                               VK_DYNAMIC_STATE_SCISSOR};

  VkPipelineDynamicStateCreateInfo dynamicState{};
  dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
  dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
  dynamicState.pDynamicStates = dynamicStates.data();

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
  rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f; // optional
  rasterizer.depthBiasClamp = 0.0f;          // optional
  rasterizer.depthBiasSlopeFactor = 0.0f;    // optional

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_FALSE;
  multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisampling.minSampleShading = 1.0f;          // optional
  multisampling.pSampleMask = nullptr;            // optional
  multisampling.alphaToCoverageEnable = VK_FALSE; // optional
  multisampling.alphaToOneEnable = VK_FALSE;      // optional

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
  colorBlending.logicOp = VK_LOGIC_OP_COPY; // optional
  colorBlending.attachmentCount = 1;
  colorBlending.pAttachments = &colorBlendAttachment;
  colorBlending.blendConstants[0] = 0.0f; // optional
  colorBlending.blendConstants[1] = 0.0f; // optional
  colorBlending.blendConstants[2] = 0.0f; // optional
  colorBlending.blendConstants[3] = 0.0f; // optional

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;            // optional
  pipelineLayoutInfo.pSetLayouts = nullptr;         // optional
  pipelineLayoutInfo.pushConstantRangeCount = 0;    // optional
  pipelineLayoutInfo.pPushConstantRanges = nullptr; // optional

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
  pipelineInfo.pDepthStencilState = nullptr; // optional
  pipelineInfo.pColorBlendState = &colorBlending;
  pipelineInfo.pDynamicState = &dynamicState;
  pipelineInfo.layout = mPipelineLayout;
  pipelineInfo.renderPass = mRenderPass;
  pipelineInfo.subpass = 0;
  pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // optional
  pipelineInfo.basePipelineIndex = -1;              // optional

  if (vkCreateGraphicsPipelines(mDevice, VK_NULL_HANDLE, 1, &pipelineInfo,
                                nullptr, &mGraphicsPipeline) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipeline.");
  }

  vkDestroyShaderModule(mDevice, vertModule, nullptr);
  vkDestroyShaderModule(mDevice, fragModule, nullptr);
}

void VulkanTriangle::createFramebuffers() {
  mSwapchainFramebuffers.resize(mSwapchainImageViews.size());
  for (size_t i = 0; i < mSwapchainImageViews.size(); ++i) {
    VkImageView attachments[] = {mSwapchainImageViews[i]};

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = mRenderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = mSwapchainProperty.extent.width;
    framebufferInfo.height = mSwapchainProperty.extent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(mDevice, &framebufferInfo, nullptr,
                            &mSwapchainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create framebuffer.");
    }
  }
}

void VulkanTriangle::createCommandPool() {
  QueueFamilyIndices indices = this->findQueueFamilies(mPhysicalDevice);
  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  poolInfo.queueFamilyIndex = indices.graphicsFamily.value();

  if (vkCreateCommandPool(mDevice, &poolInfo, nullptr, &mCommandPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create command pool.");
  }
}

void VulkanTriangle::createCommandBuffer() {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = mCommandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = 1;

  if (vkAllocateCommandBuffers(mDevice, &allocInfo, &mCommandBuffer) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffer.");
  }
}

void VulkanTriangle::recordCommandBuffer(VkCommandBuffer cmdBuffer,
                                         size_t imgIndex) {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = 0;                  // optional
  beginInfo.pInheritanceInfo = nullptr; // optional

  if (vkBeginCommandBuffer(cmdBuffer, &beginInfo) != VK_SUCCESS) {
    throw std::runtime_error("Failed to begin recording command buffer.");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = mRenderPass;
  renderPassInfo.framebuffer = mSwapchainFramebuffers[imgIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = mSwapchainProperty.extent;

  VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
  renderPassInfo.clearValueCount = 1;
  renderPassInfo.pClearValues = &clearColor;

  vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                    mGraphicsPipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = static_cast<float>(mSwapchainProperty.extent.width);
  viewport.height = static_cast<float>(mSwapchainProperty.extent.height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = mSwapchainProperty.extent;
  vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

  vkCmdDraw(cmdBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(cmdBuffer);
  if (vkEndCommandBuffer(cmdBuffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer.");
  }
}

void VulkanTriangle::createSyncObjects() {
  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  if (vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr,
                        &mImageAvailableSemaphore) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create image available semaphore.");
  }

  if (vkCreateSemaphore(mDevice, &semaphoreInfo, nullptr,
                        &mRenderFinishedSemaphore) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create render finished semaphore.");
  }

  if (vkCreateFence(mDevice, &fenceInfo, nullptr, &mInFlightFence) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create in flight fence.");
  }
}

void VulkanTriangle::drawFrame() {
  vkWaitForFences(mDevice, 1, &mInFlightFence, VK_TRUE, UINT64_MAX);
  vkResetFences(mDevice, 1, &mInFlightFence);

  uint32_t imageIndex;
  vkAcquireNextImageKHR(mDevice, mSwapchain, UINT64_MAX,
                        mImageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
  vkResetCommandBuffer(mCommandBuffer, 0);
  this->recordCommandBuffer(mCommandBuffer, imageIndex);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore waitSemaphores[] = {mImageAvailableSemaphore};
  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = 1;
  submitInfo.pWaitSemaphores = waitSemaphores;
  submitInfo.pWaitDstStageMask = waitStages;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &mCommandBuffer;

  VkSemaphore signalSemaphores[] = {mRenderFinishedSemaphore};
  submitInfo.signalSemaphoreCount = 1;
  submitInfo.pSignalSemaphores = signalSemaphores;

  if (vkQueueSubmit(mGraphicsQueue, 1, &submitInfo, mInFlightFence) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to submit draw command buffer.");
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount = 1;
  presentInfo.pWaitSemaphores = signalSemaphores;

  VkSwapchainKHR swapchains[] = {mSwapchain};
  presentInfo.swapchainCount = 1;
  presentInfo.pSwapchains = swapchains;
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = nullptr; // optional

  vkQueuePresentKHR(mPresentQueue, &presentInfo);
}

void VulkanTriangle::initVulkan() {
  this->createInstance();
  // this->setupDebugMessenger();
  mDebugger->initDebugMessenger(mInstance);
  this->createSurface();
  this->pickPhysicalDevice();
  this->createLogicalDevice();
  this->createSwapchain();
  this->createImageViews();
  this->createRenderPass();
  this->createGraphicsPipeline();
  this->createFramebuffers();
  this->createCommandPool();
  this->createCommandBuffer();
  this->createSyncObjects();
}

void VulkanTriangle::mainLoop() {
  while (!glfwWindowShouldClose(mWindow)) {
    glfwPollEvents();
    this->drawFrame();
  }

  vkDeviceWaitIdle(mDevice);
}

void VulkanTriangle::cleanup() {
  vkDestroySemaphore(mDevice, mRenderFinishedSemaphore, nullptr);
  vkDestroySemaphore(mDevice, mImageAvailableSemaphore, nullptr);
  vkDestroyFence(mDevice, mInFlightFence, nullptr);
  vkDestroyCommandPool(mDevice, mCommandPool, nullptr);
  for (auto framebuffer : mSwapchainFramebuffers) {
    vkDestroyFramebuffer(mDevice, framebuffer, nullptr);
  }
  vkDestroyPipeline(mDevice, mGraphicsPipeline, nullptr);
  vkDestroyPipelineLayout(mDevice, mPipelineLayout, nullptr);
  vkDestroyRenderPass(mDevice, mRenderPass, nullptr);
  for (auto imageView : mSwapchainImageViews) {
    vkDestroyImageView(mDevice, imageView, nullptr);
  }
  vkDestroySwapchainKHR(mDevice, mSwapchain, nullptr);
  vkDestroyDevice(mDevice, nullptr);
  if (mDebugMessenger != nullptr) {
    trDestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
  }
  vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
  vkDestroyInstance(mInstance, nullptr);
  glfwDestroyWindow(mWindow);
  glfwTerminate();
}

VulkanTriangle::VulkanTriangle() {
  this->initWindow();
  this->initVulkan();
}

VulkanTriangle::~VulkanTriangle() { this->cleanup(); }

void VulkanTriangle::run() { this->mainLoop(); }

int main() {
  VulkanTriangle app;

  try {
    app.run();
  } catch (const std::runtime_error &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return EXIT_FAILURE;
  } catch (...) {
    std::cerr << "Unknown error occurred." << std::endl;
    return EXIT_FAILURE;
  }

  std::cout << "Vulkan Triangle application finished successfully."
            << std::endl;
  return EXIT_SUCCESS;
}
