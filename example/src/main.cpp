#include "../include/main.hpp"

#include <iostream>

static constexpr u32 MAX_FRAMES_IN_FLIGHT = 2;
static constexpr char const *ENGINE_NAME = "Terreate";
static constexpr u32 ENGINE_VERSION = VK_MAKE_VERSION(0, 1, 0);

vec<char> readFile(str const &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    throw std::runtime_error("Failed to open file.");
  }

  size_t fsize = (size_t)file.tellg();
  vec<char> buffer(fsize);
  file.seekg(0);
  file.read(buffer.data(), (std::streamsize)fsize);
  file.close();

  return buffer;
}

bool Vertex::operator==(Vertex const &other) const {
  return pos == other.pos && color == other.color && texCoord == other.texCoord;
}

VkVertexInputBindingDescription Vertex::getBindingDescription() {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}

vec<VkVertexInputAttributeDescription> Vertex::getAttributeDescriptions() {
  vec<VkVertexInputAttributeDescription> attributeDescriptions;
  attributeDescriptions.resize(3);

  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, pos);

  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);

  attributeDescriptions[2].binding = 0;
  attributeDescriptions[2].location = 2;
  attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
  attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

  return attributeDescriptions;
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

VkFormat App::findSupportedFormat(vec<VkFormat> const &candidates,
                                  VkImageTiling tiling,
                                  VkFormatFeatureFlags features) {
  for (VkFormat format : candidates) {
    VkFormatProperties props = mDevice->getFormatProperties(format);

    if (tiling == VK_IMAGE_TILING_LINEAR &&
        (props.linearTilingFeatures & features) == features) {
      return format;
    } else if (tiling == VK_IMAGE_TILING_OPTIMAL &&
               (props.optimalTilingFeatures & features) == features) {
      return format;
    }
  }

  throw std::runtime_error("Failed to find supported format.");
}

VkFormat App::findDepthFormat() {
  return this->findSupportedFormat(
      {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT,
       VK_FORMAT_D24_UNORM_S8_UINT},
      VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool App::hasStencilComponent(VkFormat format) {
  return format == VK_FORMAT_D32_SFLOAT_S8_UINT ||
         format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void App::createRenderPass() {
  VkAttachmentDescription colorAttachment{};
  colorAttachment.format = mSwapchain->getImageFormat();
  colorAttachment.samples = mDevice->getMaxSampleCount();
  colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentReference colorAttachmentRef{};
  colorAttachmentRef.attachment = 0;
  colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription depthAttachment{};
  depthAttachment.format = this->findDepthFormat();
  depthAttachment.samples = mDevice->getMaxSampleCount();
  depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depthAttachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentReference depthAttachmentRef{};
  depthAttachmentRef.attachment = 1;
  depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

  VkAttachmentDescription colorAttachmentResolve{};
  colorAttachmentResolve.format = mSwapchain->getImageFormat();
  colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
  colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

  VkAttachmentReference colorAttachmentResolveRef{};
  colorAttachmentResolveRef.attachment = 2;
  colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments = &colorAttachmentRef;
  subpass.pDepthStencilAttachment = &depthAttachmentRef;
  subpass.pResolveAttachments = &colorAttachmentResolveRef;

  VkSubpassDependency dependency{};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT |
                            VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                             VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

  array<VkAttachmentDescription, 3> attachments = {
      colorAttachment, depthAttachment, colorAttachmentResolve};

  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = static_cast<u32>(attachments.size());
  renderPassInfo.pAttachments = attachments.data();
  renderPassInfo.subpassCount = 1;
  renderPassInfo.pSubpasses = &subpass;
  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &dependency;

  if (vkCreateRenderPass(*mDevice, &renderPassInfo, nullptr, &mRenderPass) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create RenderPass.");
  }
}

void App::createDescriptorSetLayout() {
  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

  VkDescriptorSetLayoutBinding samplerLayoutBinding{};
  samplerLayoutBinding.binding = 1;
  samplerLayoutBinding.descriptorCount = 1;
  samplerLayoutBinding.descriptorType =
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  samplerLayoutBinding.pImmutableSamplers = nullptr; // Optional
  samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

  vec<VkDescriptorSetLayoutBinding> bindings = {uboLayoutBinding,
                                                samplerLayoutBinding};
  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = static_cast<u32>(bindings.size());
  layoutInfo.pBindings = bindings.data();

  if (vkCreateDescriptorSetLayout(*mDevice, &layoutInfo, nullptr,
                                  &mDescriptorSetLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor set layout.");
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

  vec<VkPipelineShaderStageCreateInfo> stages = {vertInfo, fragInfo};

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
      (u32)attributeDescriptions.size();
  vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

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
  rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
  rasterizer.depthBiasEnable = VK_FALSE;
  rasterizer.depthBiasConstantFactor = 0.0f;
  rasterizer.depthBiasClamp = 0.0f;
  rasterizer.depthBiasSlopeFactor = 0.0f;

  VkPipelineMultisampleStateCreateInfo multisampling{};
  multisampling.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisampling.sampleShadingEnable = VK_TRUE;
  multisampling.rasterizationSamples = mDevice->getMaxSampleCount();
  multisampling.minSampleShading = 0.2f;
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

  VkPipelineDepthStencilStateCreateInfo depthStencil{};
  depthStencil.sType =
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
  depthStencil.depthTestEnable = VK_TRUE;
  depthStencil.depthWriteEnable = VK_TRUE;
  depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
  depthStencil.depthBoundsTestEnable = VK_FALSE;
  depthStencil.minDepthBounds = 0.0f; // Optional
  depthStencil.maxDepthBounds = 1.0f; // Optional
  depthStencil.stencilTestEnable = VK_FALSE;
  depthStencil.front = {}; // Optional
  depthStencil.back = {};  // Optional

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 1;
  pipelineLayoutInfo.pSetLayouts = &mDescriptorSetLayout;
  pipelineLayoutInfo.pushConstantRangeCount = 0;
  pipelineLayoutInfo.pPushConstantRanges = nullptr;

  if (vkCreatePipelineLayout(*mDevice, &pipelineLayoutInfo, nullptr,
                             &mPipelineLayout) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create pipeline layout.");
  }

  VkGraphicsPipelineCreateInfo pipelineInfo{};
  pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
  pipelineInfo.stageCount = static_cast<u32>(stages.size());
  pipelineInfo.pStages = stages.data();
  pipelineInfo.pVertexInputState = &vertexInputInfo;
  pipelineInfo.pInputAssemblyState = &inputAssembly;
  pipelineInfo.pViewportState = &viewportState;
  pipelineInfo.pRasterizationState = &rasterizer;
  pipelineInfo.pMultisampleState = &multisampling;
  pipelineInfo.pDepthStencilState = &depthStencil;
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

void App::createColorResources() {
  VkFormat colorFormat = mSwapchain->getImageFormat();
  VkExtent2D extent = mSwapchain->getExtent();
  mColorImage = Vulkan::Image::create(
      mDevice, mAllocator, {extent.width, extent.height}, 1,
      mDevice->getMaxSampleCount(), colorFormat, VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT |
          VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  mColorImageView = Vulkan::ImageView::create(mDevice, mColorImage, colorFormat,
                                              VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

void App::createDepthResources() {
  VkFormat depthFormat = this->findDepthFormat();
  VkExtent2D extent = mSwapchain->getExtent();
  mDepthImage = Vulkan::Image::create(
      mDevice, mAllocator, {extent.width, extent.height}, 1,
      mDevice->getMaxSampleCount(), depthFormat, VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  mDepthImageView = Vulkan::ImageView::create(mDevice, mDepthImage, depthFormat,
                                              VK_IMAGE_ASPECT_DEPTH_BIT, 1);
  this->transitionImageLayout(
      *mDepthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED,
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
}

void App::createFramebuffers() {
  mSwapchainFramebuffers.resize(mSwapchain->getImageCount());
  VkExtent2D extent = mSwapchain->getExtent();

  vec<shared<Vulkan::ImageView>> const &swapchainImageViews =
      mSwapchain->getImageViews();
  for (u32 i = 0; i < mSwapchain->getImageCount(); ++i) {
    vec<VkImageView> attachments = {
        *mColorImageView,
        *mDepthImageView,
        *swapchainImageViews[i],
    };
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = mRenderPass;
    framebufferInfo.attachmentCount = static_cast<u32>(attachments.size());
    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(*mDevice, &framebufferInfo, nullptr,
                            &mSwapchainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create framebuffer.");
    }
  }
}

u32 App::findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties =
      mDevice->getMemoryProperties();

  for (u32 i = 0; i < memProperties.memoryTypeCount; ++i) {
    if ((bool)(typeFilter & (1 << i)) &&
        (memProperties.memoryTypes[i].propertyFlags & properties) ==
            properties) {
      return i;
    }
  }

  throw std::runtime_error("Failed to find suitable memory type.");
}

void App::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                       VkMemoryPropertyFlags properties, VkBuffer &buffer,
                       VkDeviceMemory &bufferMemory) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  bufferInfo.flags = 0; // Optional

  if (vkCreateBuffer(*mDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
    throw std::runtime_error("Failed to create vertex buffer.");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(*mDevice, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      this->findMemoryType(memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(*mDevice, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate vertex buffer memory.");
  }

  vkBindBufferMemory(*mDevice, buffer, bufferMemory, 0);
}

shared<Vulkan::CommandBufferEncoder> App::beginSingleTimeCommands() {
  shared<Vulkan::CommandBuffer> commandBuffer = Vulkan::CommandBuffer::create(
      mDevice, mCommandPool, Vulkan::CommandBufferLevel::PRIMARY);
  shared<Vulkan::CommandBufferEncoder> encoder =
      Vulkan::CommandBufferEncoder::create(commandBuffer);
  encoder->begin(Vulkan::CommandBufferUsage::ONE_TIME_SUBMIT);

  return encoder;
}

void App::endSingleTimeCommands(
    shared<Vulkan::CommandBufferEncoder> const &commandBuffer) {
  commandBuffer->end();

  mQueue->submit(
      {commandBuffer->getCommandBuffer()}); // TODO: change to use fences
  mQueue->wait();
}

void App::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer,
                     VkDeviceSize size) {
  shared<Vulkan::CommandBufferEncoder> encoder =
      this->beginSingleTimeCommands();

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0;
  copyRegion.dstOffset = 0;
  copyRegion.size = size;
  vkCmdCopyBuffer(*encoder->getCommandBuffer(), srcBuffer, dstBuffer, 1,
                  &copyRegion);

  this->endSingleTimeCommands(encoder);
}

void App::transitionImageLayout(VkImage image, VkFormat format,
                                VkImageLayout oldLayout,
                                VkImageLayout newLayout, u32 mipLevels) {
  shared<Vulkan::CommandBufferEncoder> encoder =
      this->beginSingleTimeCommands();

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.oldLayout = oldLayout;
  barrier.newLayout = newLayout;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.image = image;
  barrier.subresourceRange.baseMipLevel = 0;
  barrier.subresourceRange.levelCount = mipLevels;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;
  barrier.srcAccessMask = 0;
  barrier.dstAccessMask = 0;

  if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

    if (this->hasStencilComponent(format)) {
      barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
  } else {
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  }

  VkPipelineStageFlags sourceStage;
  VkPipelineStageFlags destinationStage;

  if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED &&
      newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
  } else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
  } else if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
    barrier.srcAccessMask = 0;
    barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT |
                            VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
    destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
  } else {
    throw std::invalid_argument("Unsupported layout transition.");
  }

  encoder->pipelineBarrier(sourceStage, destinationStage, 0, {}, {}, {barrier});

  this->endSingleTimeCommands(encoder);
}

void App::copyBufferToImage(VkBuffer buffer, VkImage image, u32 width,
                            u32 height) {
  shared<Vulkan::CommandBufferEncoder> encoder =
      this->beginSingleTimeCommands();

  VkBufferImageCopy region{};
  region.bufferOffset = 0;
  region.bufferRowLength = 0;
  region.bufferImageHeight = 0;
  region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  region.imageSubresource.mipLevel = 0;
  region.imageSubresource.baseArrayLayer = 0;
  region.imageSubresource.layerCount = 1;
  region.imageOffset = {0, 0, 0};
  region.imageExtent = {width, height, 1};

  vkCmdCopyBufferToImage(*encoder->getCommandBuffer(), buffer, image,
                         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

  this->endSingleTimeCommands(encoder);
}

void App::generateMipmaps(shared<Vulkan::Image> const &image,
                          VkFormat imageFormat, i32 texWidth, i32 texHeight,
                          u32 mipLevels) {
  VkFormatProperties formatProperties =
      mDevice->getFormatProperties(imageFormat);
  if (!(bool)(formatProperties.optimalTilingFeatures &
              VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
    throw std::runtime_error(
        "Texture image format does not support linear blitting.");
  }

  shared<Vulkan::CommandBufferEncoder> encoder =
      this->beginSingleTimeCommands();

  VkImageMemoryBarrier barrier{};
  barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
  barrier.image = *image;
  barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
  barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  barrier.subresourceRange.baseArrayLayer = 0;
  barrier.subresourceRange.layerCount = 1;
  barrier.subresourceRange.levelCount = 1;

  i32 mipWidth = texWidth;
  i32 mipHeight = texHeight;

  for (u32 i = 1; i < mipLevels; ++i) {
    barrier.subresourceRange.baseMipLevel = i - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

    encoder->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_TRANSFER_BIT, 0, {}, {},
                             {barrier});

    VkImageBlit blit{};
    blit.srcOffsets[0] = {0, 0, 0};
    blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
    blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.srcSubresource.mipLevel = i - 1;
    blit.srcSubresource.baseArrayLayer = 0;
    blit.srcSubresource.layerCount = 1;
    blit.dstOffsets[0] = {0, 0, 0};
    blit.dstOffsets[1] = {mipWidth > 1 ? mipWidth / 2 : 1,
                          mipHeight > 1 ? mipHeight / 2 : 1, 1};
    blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    blit.dstSubresource.mipLevel = i;
    blit.dstSubresource.baseArrayLayer = 0;
    blit.dstSubresource.layerCount = 1;

    encoder->blitImage(image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image,
                       VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, {blit},
                       VK_FILTER_LINEAR);

    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    encoder->pipelineBarrier(VK_PIPELINE_STAGE_TRANSFER_BIT,
                             VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, {}, {},
                             {barrier});

    if (mipWidth > 1)
      mipWidth /= 2;
    if (mipHeight > 1)
      mipHeight /= 2;
  }

  barrier.subresourceRange.baseMipLevel = mipLevels - 1;
  barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
  barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
  barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
  barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

  vkCmdPipelineBarrier(*encoder->getCommandBuffer(),
                       VK_PIPELINE_STAGE_TRANSFER_BIT,
                       VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0,
                       nullptr, 1, &barrier);

  this->endSingleTimeCommands(encoder);
}

void App::createTexture() {
  int width, height, channels;
  stbi_uc *pixels =
      stbi_load(TEXTURE_PATH, &width, &height, &channels, STBI_rgb_alpha);
  VkDeviceSize imageSize = width * height * 4;
  mMipLevels =
      static_cast<u32>(std::floor(std::log2(std::max(width, height)))) + 1;

  if (!pixels) {
    throw std::runtime_error("Failed to load texture image.");
  }

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  this->createBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(*mDevice, stagingBufferMemory, 0, imageSize, 0, &data);
  std::memcpy(data, pixels, static_cast<size_t>(imageSize));
  vkUnmapMemory(*mDevice, stagingBufferMemory);
  stbi_image_free(pixels);

  mTextureImage = Vulkan::Image::create(
      mDevice, mAllocator, {static_cast<u32>(width), static_cast<u32>(height)},
      mMipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB,
      VK_IMAGE_TILING_OPTIMAL,
      VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT |
          VK_IMAGE_USAGE_SAMPLED_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  this->transitionImageLayout(*mTextureImage, VK_FORMAT_R8G8B8A8_SRGB,
                              VK_IMAGE_LAYOUT_UNDEFINED,
                              VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, mMipLevels);
  this->copyBufferToImage(stagingBuffer, *mTextureImage,
                          static_cast<u32>(width), static_cast<u32>(height));
  this->generateMipmaps(mTextureImage, VK_FORMAT_R8G8B8A8_SRGB, width, height,
                        mMipLevels);

  vkDestroyBuffer(*mDevice, stagingBuffer, nullptr);
  vkFreeMemory(*mDevice, stagingBufferMemory, nullptr);
}

void App::createTextureImageView() {
  mTextureImageView =
      Vulkan::ImageView::create(mDevice, mTextureImage, VK_FORMAT_R8G8B8A8_SRGB,
                                VK_IMAGE_ASPECT_COLOR_BIT, mMipLevels);
}

void App::createTextureSampler() {
  VkPhysicalDeviceProperties properties = mDevice->getProperties();

  VkSamplerCreateInfo samplerInfo{};
  samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
  samplerInfo.magFilter = VK_FILTER_LINEAR;
  samplerInfo.minFilter = VK_FILTER_LINEAR;
  samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
  samplerInfo.anisotropyEnable = VK_TRUE;
  samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
  samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
  samplerInfo.unnormalizedCoordinates = VK_FALSE;
  samplerInfo.compareEnable = VK_FALSE;
  samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
  samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
  samplerInfo.mipLodBias = 0.0f;
  samplerInfo.maxLod = static_cast<float>(mMipLevels);
  samplerInfo.minLod = 0.0f;

  if (vkCreateSampler(*mDevice, &samplerInfo, nullptr, &mTextureSampler) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create texture sampler.");
  }
}

void App::loadModel() {
  tinyobj::attrib_t attrib;
  vec<tinyobj::shape_t> shapes;
  vec<tinyobj::material_t> materials;
  str warn, err;

  if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                        MODEL_PATH)) {
    throw std::runtime_error(warn + err);
  }

  for (auto const &shape : shapes) {
    umap<Vertex, u32> uniqueVertices;
    for (auto const &index : shape.mesh.indices) {
      Vertex vertex{};

      vertex.pos = {
          attrib.vertices[3 * index.vertex_index + 0],
          attrib.vertices[3 * index.vertex_index + 1],
          attrib.vertices[3 * index.vertex_index + 2],
      };
      vertex.texCoord = {
          attrib.texcoords[2 * index.texcoord_index + 0],
          1.0f - attrib.texcoords[2 * index.texcoord_index + 1],
      };
      vertex.color = {1.0f, 1.0f, 1.0f};

      if (!uniqueVertices.contains(vertex)) {
        uniqueVertices[vertex] = static_cast<u32>(mVertices.size());
        mVertices.push_back(vertex);
      }

      mIndices.push_back(uniqueVertices[vertex]);
    }
  }
}

void App::createVertexBuffer() {
  VkDeviceSize bufferSize = mVertices.size() * sizeof(mVertices[0]);

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  this->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(*mDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
  std::memcpy(data, mVertices.data(), (size_t)bufferSize);
  vkUnmapMemory(*mDevice, stagingBufferMemory);

  this->createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mVertexBuffer, mVertexBufferMemory);
  this->copyBuffer(stagingBuffer, mVertexBuffer, bufferSize);

  vkDestroyBuffer(*mDevice, stagingBuffer, nullptr);
  vkFreeMemory(*mDevice, stagingBufferMemory, nullptr);
}

void App::createIndexBuffer() {
  VkDeviceSize bufferSize = sizeof(mIndices[0]) * mIndices.size();

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  this->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                         VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                     stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(*mDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
  std::memcpy(data, mIndices.data(), (size_t)bufferSize);
  vkUnmapMemory(*mDevice, stagingBufferMemory);

  this->createBuffer(
      bufferSize,
      VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, mIndexBuffer, mIndexBufferMemory);
  this->copyBuffer(stagingBuffer, mIndexBuffer, bufferSize);

  vkDestroyBuffer(*mDevice, stagingBuffer, nullptr);
  vkFreeMemory(*mDevice, stagingBufferMemory, nullptr);
}

void App::createUniformBuffers() {
  VkDeviceSize bufferSize = sizeof(UniformBufferObject);

  mUniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
  mUniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
  mUniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    this->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                           VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                       mUniformBuffers[i], mUniformBuffersMemory[i]);
    vkMapMemory(*mDevice, mUniformBuffersMemory[i], 0, bufferSize, 0,
                &mUniformBuffersMapped[i]);
  }
}

void App::createCommandBuffers() {
  mCommandBufferEncoders.clear();
  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    shared<Vulkan::CommandBuffer> commandBuffer = Vulkan::CommandBuffer::create(
        mDevice, mCommandPool, Vulkan::CommandBufferLevel::PRIMARY);
    mCommandBufferEncoders.push_back(
        Vulkan::CommandBufferEncoder::create(commandBuffer));
  }
}

void App::createDescriptorPool() {
  vec<VkDescriptorPoolSize> poolSizes;
  poolSizes.resize(2);

  poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSizes[0].descriptorCount = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);
  poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
  poolSizes[1].descriptorCount = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);

  VkDescriptorPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  poolInfo.poolSizeCount = static_cast<u32>(poolSizes.size());
  poolInfo.pPoolSizes = poolSizes.data();
  poolInfo.maxSets = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);

  if (vkCreateDescriptorPool(*mDevice, &poolInfo, nullptr, &mDescriptorPool) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to create descriptor pool.");
  }
}

void App::createDescriptorSets() {
  vec<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT,
                                     mDescriptorSetLayout);
  VkDescriptorSetAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
  allocInfo.descriptorPool = mDescriptorPool;
  allocInfo.descriptorSetCount = static_cast<u32>(MAX_FRAMES_IN_FLIGHT);
  allocInfo.pSetLayouts = layouts.data();

  mDescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
  if (vkAllocateDescriptorSets(*mDevice, &allocInfo, mDescriptorSets.data()) !=
      VK_SUCCESS) {
    throw std::runtime_error("Failed to allocate descriptor sets.");
  }

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = mUniformBuffers[i];
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView = *mTextureImageView;
    imageInfo.sampler = mTextureSampler;

    vec<VkWriteDescriptorSet> descriptorWrites;
    descriptorWrites.resize(2);

    descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[0].dstSet = mDescriptorSets[i];
    descriptorWrites[0].dstBinding = 0;
    descriptorWrites[0].dstArrayElement = 0;
    descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrites[0].descriptorCount = 1;
    descriptorWrites[0].pBufferInfo = &bufferInfo;
    descriptorWrites[0].pImageInfo = nullptr;       // Optional
    descriptorWrites[0].pTexelBufferView = nullptr; // Optional

    descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrites[1].dstSet = mDescriptorSets[i];
    descriptorWrites[1].dstBinding = 1;
    descriptorWrites[1].dstArrayElement = 0;
    descriptorWrites[1].descriptorType =
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWrites[1].descriptorCount = 1;
    descriptorWrites[1].pBufferInfo = nullptr; // Optional
    descriptorWrites[1].pImageInfo = &imageInfo;
    descriptorWrites[1].pTexelBufferView = nullptr; // Optional

    vkUpdateDescriptorSets(*mDevice, static_cast<u32>(descriptorWrites.size()),
                           descriptorWrites.data(), 0, nullptr);
  }
}

void App::recordCommandBuffer(
    shared<Vulkan::CommandBufferEncoder> const &encoder, u32 imageIndex) {
  encoder->begin(Vulkan::CommandBufferUsage::NONE);

  VkExtent2D extent = mSwapchain->getExtent();
  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = mRenderPass;
  renderPassInfo.framebuffer = mSwapchainFramebuffers[imageIndex];
  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = extent;

  vec<VkClearValue> clearValues{};
  clearValues.resize(2);

  clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<u32>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  encoder->beginRenderPass(renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(*encoder->getCommandBuffer(),
                    VK_PIPELINE_BIND_POINT_GRAPHICS, mGraphicsPipeline);

  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;
  viewport.width = (float)extent.width;
  viewport.height = (float)extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  encoder->setViewport(viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extent;
  encoder->setScissor(scissor);

  array<VkBuffer, 1> vertexBuffers = {mVertexBuffer};
  array<VkDeviceSize, 1> offsets = {0};
  vkCmdBindVertexBuffers(*encoder->getCommandBuffer(), 0, 1,
                         vertexBuffers.data(), offsets.data());
  vkCmdBindIndexBuffer(*encoder->getCommandBuffer(), mIndexBuffer, 0,
                       VK_INDEX_TYPE_UINT32);

  vkCmdBindDescriptorSets(*encoder->getCommandBuffer(),
                          VK_PIPELINE_BIND_POINT_GRAPHICS, mPipelineLayout, 0,
                          1, &mDescriptorSets[mCurrentFrame], 0, nullptr);
  encoder->drawIndexed(static_cast<u32>(mIndices.size()), 1, 0, 0, 0);

  encoder->endRenderPass();
  encoder->end();
}

void App::createSyncObjects() {
  mImageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
  mRenderFinishedSemaphores.resize(mSwapchain->getImageCount());
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

  for (u32 i = 0; i < mSwapchain->getImageCount(); ++i) {
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
}

void App::recreateSwapchain() {
  pair<i32> size = mWindow->getSize();
  while (size.first == 0 || size.second == 0) {
    size = mWindow->getSize();
    SDL_WaitEvent(nullptr);
  }

  mDevice->waitIdle();

  mSwapchain->update();
  this->cleanupSwapchain();
  this->createColorResources();
  this->createDepthResources();
  this->createFramebuffers();
}

void App::initVulkan() {
  mDevice = Vulkan::Device::create(mInstance, mWindow);
  mQueue = Vulkan::Queue::create(mDevice);
  mSwapchain = Vulkan::Swapchain::create(mDevice, mWindow);
  mAllocator = Vulkan::Allocator::create(mInstance, mDevice);

  mCommandPool = Vulkan::CommandPool::create(mDevice);
  this->createCommandBuffers();

  this->createRenderPass();
  this->createDescriptorSetLayout();
  this->createGraphicsPipeline();

  this->createColorResources();
  this->createDepthResources();
  this->createFramebuffers();

  this->createTexture();
  this->createTextureImageView();
  this->createTextureSampler();

  this->loadModel();

  this->createVertexBuffer();
  this->createIndexBuffer();
  this->createUniformBuffers();

  this->createDescriptorPool();
  this->createDescriptorSets();

  this->createSyncObjects();
}

bool App::pollEvents() {
  SDL_Event handle;
  while (SDL_PollEvent(&handle)) {
    switch (handle.type) {
    case SDL_EVENT_QUIT:
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

void App::updateUniformBuffer(u32 currentImage) {
  static auto startTime = std::chrono::high_resolution_clock::now();

  VkExtent2D extent = mSwapchain->getExtent();
  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();
  time /= 4;

  UniformBufferObject ubo{};
  float scale = (std::sin(time) + 1.0f) / 4.0f + 0.5f;
  float angle = time * glm::radians(90.0f);
  auto model = glm::mat4(1.0f);
  model = glm::scale(model, glm::vec3(scale, scale, scale));
  model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.model = model;
  ubo.view =
      glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  ubo.proj =
      glm::perspective(glm::radians(45.0f),
                       (float)extent.width / (float)extent.height, 0.1f, 10.0f);
  ubo.proj[1][1] *= -1;
  std::memcpy(mUniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void App::drawFrame() {
  vkWaitForFences(*mDevice, 1, &mInFlightFences[mCurrentFrame], VK_TRUE,
                  UINT64_MAX);

  i32 imageIndex =
      mSwapchain->getNextImageIndex(mImageAvailableSemaphores[mCurrentFrame]);

  if (imageIndex == -1) {
    this->recreateSwapchain();
    return;
  }

  vkResetFences(*mDevice, 1, &mInFlightFences[mCurrentFrame]);

  mCommandBufferEncoders[mCurrentFrame]->reset();
  this->recordCommandBuffer(mCommandBufferEncoders[mCurrentFrame], imageIndex);

  this->updateUniformBuffer(mCurrentFrame);

  mQueue->submit({mImageAvailableSemaphores[mCurrentFrame]},
                 {mRenderFinishedSemaphores[imageIndex]},
                 {mCommandBufferEncoders[mCurrentFrame]->getCommandBuffer()},
                 mInFlightFences[mCurrentFrame]);

  if (!mQueue->present(mSwapchain, imageIndex,
                       {mRenderFinishedSemaphores[imageIndex]})) {
    this->recreateSwapchain();
  }

  if (mFramebufferResized) {
    this->recreateSwapchain();
    mFramebufferResized = false;
  }

  mCurrentFrame = (mCurrentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void App::mainLoop() {
  while (this->pollEvents()) {
    this->drawFrame();
  }

  mDevice->waitIdle();
}

void App::cleanup() {
  this->cleanupSwapchain();

  vkDestroySampler(*mDevice, mTextureSampler, nullptr);

  for (u32 i = 0; i < MAX_FRAMES_IN_FLIGHT; ++i) {
    vkUnmapMemory(*mDevice, mUniformBuffersMemory[i]);
    vkDestroyBuffer(*mDevice, mUniformBuffers[i], nullptr);
    vkFreeMemory(*mDevice, mUniformBuffersMemory[i], nullptr);
  }

  vkDestroyDescriptorPool(*mDevice, mDescriptorPool, nullptr);
  vkDestroyDescriptorSetLayout(*mDevice, mDescriptorSetLayout, nullptr);

  vkDestroyBuffer(*mDevice, mVertexBuffer, nullptr);
  vkFreeMemory(*mDevice, mVertexBufferMemory, nullptr);

  vkDestroyBuffer(*mDevice, mIndexBuffer, nullptr);
  vkFreeMemory(*mDevice, mIndexBufferMemory, nullptr);

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

  for (u32 i = 0; i < mSwapchain->getImageCount(); ++i) {
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
  try {
    App app;
    app.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  SDL_Quit();
  return EXIT_SUCCESS;
}
