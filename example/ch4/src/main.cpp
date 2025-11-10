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

void App::createImageViews() {
  mSwapchainImageViews.resize(mSwapchain->getImageCount());

  vec<VkImage> const &swapchainImages = mSwapchain->getImages();
  for (u32 i = 0; i < swapchainImages.size(); ++i) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image = swapchainImages[i];
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = mSwapchain->getImageFormat();
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
  colorAttachment.format = mSwapchain->getImageFormat();
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
  VkExtent2D extent = mSwapchain->getExtent();

  for (u32 i = 0; i < mSwapchainImageViews.size(); ++i) {
    VkImageView attachments[] = {mSwapchainImageViews[i]};
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = mRenderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = attachments;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    if (vkCreateFramebuffer(*mDevice, &framebufferInfo, nullptr,
                            &mSwapchainFramebuffers[i]) != VK_SUCCESS) {
      throw std::runtime_error("Failed to create framebuffer.");
    }
  }
}

void App::createCommandPool() {
  umap<Vulkan::QueueType, u32> queueFamilyIndices =
      mDevice->getQueueFamilyIndices();

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
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
  VkExtent2D extent = mSwapchain->getExtent();

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
  renderPassInfo.renderArea.extent = extent;

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
  viewport.width = (float)extent.width;
  viewport.height = (float)extent.height;
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

  VkRect2D scissor{};
  scissor.offset = {0, 0};
  scissor.extent = extent;
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
}

void App::recreateSwapchain() {
  pair<i32> size = mWindow->getSize();
  while (size.first == 0 || size.second == 0) {
    size = mWindow->getSize();
    SDL_WaitEvent(nullptr);
  }

  vkDeviceWaitIdle(*mDevice);

  mSwapchain->update();
  this->cleanupSwapchain();
  this->createImageViews();
  this->createFramebuffers();
}

void App::initVulkan() {
  mDevice = Vulkan::Device::create(mInstance, mWindow);
  mQueue = Vulkan::Queue::create(mDevice);
  mSwapchain = Vulkan::Swapchain::create(mDevice, mWindow);
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

  i32 imageIndex =
      mSwapchain->getNextImageIndex(mImageAvailableSemaphores[mCurrentFrame]);

  if (imageIndex == -1) {
    this->recreateSwapchain();
    return;
  }

  vkResetFences(*mDevice, 1, &mInFlightFences[mCurrentFrame]);

  vkResetCommandBuffer(mCommandBuffers[mCurrentFrame], 0);
  this->recordCommandBuffer(mCommandBuffers[mCurrentFrame], imageIndex);

  mQueue->submit({mImageAvailableSemaphores[mCurrentFrame]},
                 {mRenderFinishedSemaphores[imageIndex]},
                 {mCommandBuffers[mCurrentFrame]},
                 mInFlightFences[mCurrentFrame]);

  if (!mQueue->present(*mSwapchain, imageIndex,
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
