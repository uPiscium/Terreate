#include "../include/main.hpp"

template <typename T, size_t N> using array = std::array<T, N>;

static constexpr u32 MAX_FRAMES_IN_FLIGHT = 2;
static constexpr char const *ENGINE_NAME = "Terreate";
static constexpr u32 ENGINE_VERSION = VK_MAKE_VERSION(0, 1, 0);

static constexpr char const *VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"};
static constexpr char const *DEVICE_EXTENSIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

#include <iostream>

vec<char> readFile(str const &filename) {
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

void App::initWindow(int const &width, int const &height, str const &title) {
  shared<SDL::Mouse> mouse;
  if (mSDLRegistry->hasMouse(0)) {
    mouse = mSDLRegistry->getMouse(0);
  } else {
    mouse = std::make_shared<SDL::Mouse>(0);
    mSDLRegistry->registerMouse(0, mouse);
  }

  mWindow = SDL::Window::create(mInstance, width, height, title, mouse);
}

// QueueFamilyIndices App::findQueueFamilies(VkPhysicalDevice device) {
//   QueueFamilyIndices indices;

//   u32 queueFamilyCount = 0;
//   vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
//   nullptr); vec<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//   vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,
//                                            queueFamilies.data());

//   u32 index = 0;
//   for (auto const &queueFamily : queueFamilies) {
//     if (queueFamily.queueCount > 0 &&
//         queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
//       indices.graphicsFamily = index;
//     }

//     VkBool32 presentSupport = false;
//     vkGetPhysicalDeviceSurfaceSupportKHR(device, 0, *mWindow,
//     &presentSupport); if (presentSupport) {
//       indices.presentFamily = index;
//     }

//     if (indices.isComplete()) {
//       break;
//     }

//     ++index;
//   }

//   return indices;
// }

// bool App::checkDeviceExtensionSupport(VkPhysicalDevice device) {
//   u32 extensionCount;
//   vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
//                                        nullptr);

//   vec<VkExtensionProperties> availableExtensions(extensionCount);
//   vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount,
//                                        availableExtensions.data());

//   u32 length = sizeof(DEVICE_EXTENSIONS) / sizeof(DEVICE_EXTENSIONS[0]);
//   uset<str> requiredExtensions(DEVICE_EXTENSIONS, DEVICE_EXTENSIONS +
//   length);

//   for (auto const &extension : availableExtensions) {
//     requiredExtensions.erase(extension.extensionName);
//   }

//   return requiredExtensions.empty();
// }

// SwapChainSupportDetails App::querySwapChainSupport(VkPhysicalDevice device) {
//   SwapChainSupportDetails details;
//   vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *mWindow,
//                                             &details.capabilities);

//   u32 formatCount;
//   vkGetPhysicalDeviceSurfaceFormatsKHR(device, *mWindow, &formatCount,
//   nullptr); if (formatCount != 0) {
//     details.formats.resize(formatCount);
//     vkGetPhysicalDeviceSurfaceFormatsKHR(device, *mWindow, &formatCount,
//                                          details.formats.data());
//   }

//   u32 presentModeCount;
//   vkGetPhysicalDeviceSurfacePresentModesKHR(device, *mWindow,
//   &presentModeCount,
//                                             nullptr);
//   if (presentModeCount != 0) {
//     details.presentModes.resize(presentModeCount);
//     vkGetPhysicalDeviceSurfacePresentModesKHR(
//         device, *mWindow, &presentModeCount, details.presentModes.data());
//   }

//   return details;
// }

// int App::rateDevice(VkPhysicalDevice device) {
//   if (!this->findQueueFamilies(device).isComplete()) {
//     return -1;
//   }

//   if (!this->checkDeviceExtensionSupport(device)) {
//     return -1;
//   }

//   SwapChainSupportDetails swapChainSupport =
//       this->querySwapChainSupport(device);
//   if (swapChainSupport.formats.empty() ||
//       swapChainSupport.presentModes.empty()) {
//     return -1;
//   }

//   int score = 0;
//   VkPhysicalDeviceProperties deviceProperties;
//   VkPhysicalDeviceFeatures deviceFeatures;
//   vkGetPhysicalDeviceProperties(device, &deviceProperties);
//   vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

//   // Discrete GPUs have a significant performance advantage
//   if (deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
//     score += 1000;
//   }

//   return score;
// }

// void App::pickPhysicalDevice() {
//   u32 deviceCount = 0;
//   vkEnumeratePhysicalDevices(*mInstance, &deviceCount, nullptr);

//   if (deviceCount == 0) {
//     throw std::runtime_error("Failed to find GPUs with Vulkan support.");
//   }

//   vec<VkPhysicalDevice> devices(deviceCount);
//   vkEnumeratePhysicalDevices(*mInstance, &deviceCount, devices.data());

//   VkPhysicalDevice best = VK_NULL_HANDLE;
//   int bestScore = -1;
//   for (auto const &device : devices) {
//     int score = this->rateDevice(device);
//     if (score > bestScore) {
//       best = device;
//       bestScore = score;
//     }
//   }

//   if (bestScore == -1 || best == VK_NULL_HANDLE) {
//     throw std::runtime_error("Failed to find a suitable GPU.");
//   }
//   mPhysicalDevice = best;
// }

// void App::createLogicalDevice() {
//   QueueFamilyIndices indices = this->findQueueFamilies(mPhysicalDevice);

//   uset<i32> uniqueQueueFamilies = indices;

//   vec<VkDeviceQueueCreateInfo> queueCreateInfos;
//   float queuePriority = 1.0f;
//   for (i32 queueFamily : uniqueQueueFamilies) {
//     VkDeviceQueueCreateInfo queueCreateInfo{};
//     queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//     queueCreateInfo.queueFamilyIndex = queueFamily;
//     queueCreateInfo.queueCount = 1;
//     queueCreateInfo.pQueuePriorities = &queuePriority;
//     queueCreateInfos.push_back(queueCreateInfo);
//   }

//   VkPhysicalDeviceFeatures deviceFeatures{};
//   VkDeviceCreateInfo createInfo{};
//   createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//   createInfo.pQueueCreateInfos = queueCreateInfos.data();
//   createInfo.queueCreateInfoCount = queueCreateInfos.size();
//   createInfo.pEnabledFeatures = &deviceFeatures;

//   createInfo.enabledExtensionCount =
//       sizeof(DEVICE_EXTENSIONS) / sizeof(DEVICE_EXTENSIONS[0]);
//   createInfo.ppEnabledExtensionNames = DEVICE_EXTENSIONS;

//   if (mDebugMode) {
//     createInfo.enabledLayerCount =
//         sizeof(VALIDATION_LAYERS) / sizeof(VALIDATION_LAYERS[0]);
//     createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
//   } else {
//     createInfo.enabledLayerCount = 0;
//   }

//   if (vkCreateDevice(mPhysicalDevice, &createInfo, nullptr, &mDevice) !=
//       VK_SUCCESS) {
//     throw std::runtime_error("Failed to create logical device.");
//   }
// }

void App::createQueue() {
  umap<Vulkan::QueueType, i32> indices = mDevice->getQueueFamilyIndices();
  vkGetDeviceQueue(*mDevice, (u32)indices[Vulkan::QueueType::GRAPHICS], 0,
                   &mGraphicsQueue);
  vkGetDeviceQueue(*mDevice, (u32)indices[Vulkan::QueueType::GRAPHICS], 0,
                   &mPresentQueue);
}

VkSurfaceFormatKHR
App::chooseSwapSurfaceFormat(vec<VkSurfaceFormatKHR> const &availableFormats) {
  for (auto const &availableFormat : availableFormats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      return availableFormat;
    }
  }

  return availableFormats[0];
}

VkPresentModeKHR
App::chooseSwapPresentMode(vec<VkPresentModeKHR> const &availablePresentModes) {
  for (auto const &availablePresentMode : availablePresentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      return availablePresentMode;
    }
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D App::chooseSwapExtent(VkSurfaceCapabilitiesKHR const &capabilities) {
  if (capabilities.currentExtent.width != UINT32_MAX) {
    return capabilities.currentExtent;
  } else {
    pair<i32> size = mWindow->getSize();
    VkExtent2D actualExtent = {static_cast<u32>(size.first),
                               static_cast<u32>(size.second)};
    actualExtent.width =
        std::clamp(actualExtent.width, capabilities.minImageExtent.width,
                   capabilities.maxImageExtent.width);
    actualExtent.height =
        std::clamp(actualExtent.height, capabilities.minImageExtent.height,
                   capabilities.maxImageExtent.height);
    return actualExtent;
  }
}

void App::createSwapchain() {
  VkSurfaceCapabilitiesKHR capabilities = mDevice->getSurfaceCapabilities();
  VkSurfaceFormatKHR surfaceFormat =
      this->chooseSwapSurfaceFormat(mDevice->getSurfaceFormats());
  VkPresentModeKHR presentMode =
      this->chooseSwapPresentMode(mDevice->getPresentModes());
  VkExtent2D extent = this->chooseSwapExtent(capabilities);
  u32 imageCount = capabilities.minImageCount + 1;
  if (capabilities.maxImageCount > 0 &&
      imageCount > capabilities.maxImageCount) {
    imageCount = capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface = *mWindow;
  createInfo.minImageCount = imageCount;
  createInfo.imageFormat = surfaceFormat.format;
  createInfo.imageColorSpace = surfaceFormat.colorSpace;
  createInfo.imageExtent = extent;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;

  createInfo.preTransform = capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode = presentMode;
  createInfo.clipped = VK_TRUE;
  createInfo.oldSwapchain = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(*mDevice, &createInfo, nullptr, &mSwapchain) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create swap chain.");
  }

  vkGetSwapchainImagesKHR(*mDevice, mSwapchain, &imageCount, nullptr);
  mSwapchainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(*mDevice, mSwapchain, &imageCount,
                          mSwapchainImages.data());

  mSwapchainImageFormat = surfaceFormat.format;
  mSwapchainExtent = extent;
}

void App::createImageViews() {
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

    if (vkCreateImageView(*mDevice, &createInfo, nullptr,
                          &mSwapchainImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create image view.");
    }
  }
}

VkShaderModule App::createShaderModule(vec<char> const &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<u32 const *>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(*mDevice, &createInfo, nullptr, &shaderModule) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create shader module.");
  }

  return shaderModule;
}

void App::createRenderPass() {
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

  if (vkCreateRenderPass(*mDevice, &renderPassInfo, nullptr, &mRenderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create RenderPass.");
  }
}

void App::createGraphicsPipeline() {
  vec<char> vert = readFile("assets/shaders/shader.vert.spv");
  vec<char> frag = readFile("assets/shaders/shader.frag.spv");

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
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(*mDevice, &pipelineLayoutInfo, nullptr,
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

  if (vkCreateGraphicsPipelines(*mDevice, nullptr, 1, &pipelineInfo, nullptr,
                                &mGraphicsPipeline) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create graphics pipeline.");
  }

  vkDestroyShaderModule(*mDevice, fragShaderModule, nullptr);
  vkDestroyShaderModule(*mDevice, vertShaderModule, nullptr);
}

void App::createFramebuffers() {
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

    if (vkCreateFramebuffer(*mDevice, &framebufferInfo, nullptr,
                            &mSwapchainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create framebuffer.");
    }
  }
}

void App::createCommandPool() {
  // QueueFamilyIndices queueFamilyIndices =
  //     this->findQueueFamilies(mPhysicalDevice);
  umap<Vulkan::QueueType, i32> queueFamilyIndices =
      mDevice->getQueueFamilyIndices();

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
  // poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
  poolInfo.queueFamilyIndex =
      (u32)queueFamilyIndices[Vulkan::QueueType::GRAPHICS];

  if (vkCreateCommandPool(*mDevice, &poolInfo, nullptr, &mCommandPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create command pool.");
  }
}

void App::createCommandBuffers() {
  mCommandBuffers.resize(MAX_FRAMES_IN_FLIGHT);

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.commandPool = mCommandPool;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandBufferCount = (u32)mCommandBuffers.size();

  if (vkAllocateCommandBuffers(*mDevice, &allocInfo, mCommandBuffers.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate command buffers.");
  }
}

void App::recordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex) {
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

  vkCmdDraw(commandBuffer, 3, 1, 0, 0);

  vkCmdEndRenderPass(commandBuffer);
  if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to record command buffer.");
  }
}

void App::createSyncObjects() {
  mImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  mRenderFinishedSemaphores.resize(mSwapchainImageViews.size());
  mInFlightFences.resize(MAX_FRAMES_IN_FLIGHT);

  VkSemaphoreCreateInfo semaphoreInfo{};
  semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo fenceInfo{};
  fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    if (vkCreateSemaphore(*mDevice, &semaphoreInfo, nullptr,
                          &mImageAvailableSemaphores[i]) != VK_SUCCESS ||
        vkCreateFence(*mDevice, &fenceInfo, nullptr, &mInFlightFences[i]) !=
            VK_SUCCESS) {
      throw std::runtime_error("Failed to create synchronization objects.");
    }
  }

  for (u32 i = 0; i < mSwapchainImageViews.size(); ++i) {
    if (vkCreateSemaphore(*mDevice, &semaphoreInfo, nullptr,
                          &mRenderFinishedSemaphores[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create synchronization objects.");
    }
  }
}

void App::cleanupSwapchain() {
  for (u32 i = 0; i < mSwapchainFramebuffers.size(); ++i) {
    vkDestroyFramebuffer(*mDevice, mSwapchainFramebuffers[i], nullptr);
  }

  for (u32 i = 0; i < mSwapchainImageViews.size(); ++i) {
    vkDestroyImageView(*mDevice, mSwapchainImageViews[i], nullptr);
  }
  vkDestroySwapchainKHR(*mDevice, mSwapchain, nullptr);
}

void App::recreateSwapchain() {
  pair<i32> size = mWindow->getSize();
  while (size.first == 0 || size.second == 0) {
    size = mWindow->getSize();
    SDL_WaitEvent(nullptr);
  }

  vkDeviceWaitIdle(*mDevice);

  this->cleanupSwapchain();
  this->createSwapchain();
  this->createImageViews();
  this->createFramebuffers();
}

void App::initVulkan() {
  // this->pickPhysicalDevice();
  // this->createLogicalDevice();
  mDevice = Vulkan::Device::create(mInstance, mWindow);
  this->createQueue();
  this->createSwapchain();
  this->createImageViews();
  this->createCommandPool();
  this->createRenderPass();
  this->createGraphicsPipeline();
  this->createFramebuffers();
  this->createCommandBuffers();
  this->createSyncObjects();
}

bool App::pollEvents() {
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

void App::drawFrame() {
  vkWaitForFences(*mDevice, 1, &mInFlightFences[mCurrentFrame], VK_TRUE,
                  UINT64_MAX);

  u32 imageIndex;
  VkResult result = vkAcquireNextImageKHR(
      *mDevice, mSwapchain, UINT64_MAX,
      mImageAvailableSemaphores[mCurrentFrame], VK_NULL_HANDLE, &imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    this->recreateSwapchain();
    return;
  } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("Failed to acquire swap chain image.");
  }

  vkResetFences(*mDevice, 1, &mInFlightFences[mCurrentFrame]);

  vkResetCommandBuffer(mCommandBuffers[mCurrentFrame], 0);
  this->recordCommandBuffer(mCommandBuffers[mCurrentFrame], imageIndex);

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

  VkSemaphore signalSemaphores[] = {mRenderFinishedSemaphores[imageIndex]};
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

void App::mainLoop() {
  while (this->pollEvents()) {
    this->drawFrame();
  }

  vkDeviceWaitIdle(*mDevice);
}

void App::cleanup() {
  this->cleanupSwapchain();

  if (mGraphicsPipeline != VK_NULL_HANDLE) {
    vkDestroyPipeline(*mDevice, mGraphicsPipeline, nullptr);
    mGraphicsPipeline = VK_NULL_HANDLE;
  }

  if (mPipelineLayout != VK_NULL_HANDLE) {
    vkDestroyPipelineLayout(*mDevice, mPipelineLayout, nullptr);
    mPipelineLayout = VK_NULL_HANDLE;
  }

  if (mRenderPass != VK_NULL_HANDLE) {
    vkDestroyRenderPass(*mDevice, mRenderPass, nullptr);
    mRenderPass = VK_NULL_HANDLE;
  }

  for (u32 i = 0; i < mSwapchainImageViews.size(); ++i) {
    if (mRenderFinishedSemaphores[i] != VK_NULL_HANDLE) {
      vkDestroySemaphore(*mDevice, mRenderFinishedSemaphores[i], nullptr);
      mRenderFinishedSemaphores[i] = VK_NULL_HANDLE;
    }
  }

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    if (mInFlightFences[i] != VK_NULL_HANDLE) {
      vkDestroyFence(*mDevice, mInFlightFences[i], nullptr);
      mInFlightFences[i] = VK_NULL_HANDLE;
    }

    if (mImageAvailableSemaphores[i] != VK_NULL_HANDLE) {
      vkDestroySemaphore(*mDevice, mImageAvailableSemaphores[i], nullptr);
      mImageAvailableSemaphores[i] = VK_NULL_HANDLE;
    }
  }

  if (mCommandPool != VK_NULL_HANDLE) {
    vkDestroyCommandPool(*mDevice, mCommandPool, nullptr);
    mCommandPool = VK_NULL_HANDLE;
  }

  // vkDestroyDevice(*mDevice, nullptr);

  SDL_Quit();
}

App::App(int const &width, int const &height, str const &title, bool debugMode)
    : mDebugMode(debugMode) {
  if (!SDL_Init(SDL_FLAGS)) {
    str msg = "Failed to initialize SDL: ";
    msg += SDL_GetError();
    throw std::runtime_error(msg);
    return;
  }

  mSDLRegistry = std::make_shared<SDL::Registry>();
  mDebugger = std::make_shared<Vulkan::DefaultDebugger>();
  mInstance = Vulkan::Instance::create("Vulkan App", {0, 0, 1}, mDebugMode);
  mInstance->attachDebugger(mDebugger);

  this->initWindow(width, height, title);
  this->initVulkan();
}

App::~App() { this->cleanup(); }

void App::run() { this->mainLoop(); }

int main() {
  App app;

  try {
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
