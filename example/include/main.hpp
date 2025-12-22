#pragma once

#include <Terreate/common.hpp>
#include <Terreate/core.hpp>

#include <tinyobjloader/tiny_obj_loader.h>

using namespace Terreate;

static char const *const TEXTURE_PATH =
    "assets/models/viking_room/viking_room.png";
static char const *const MODEL_PATH =
    "assets/models/viking_room/viking_room.obj";

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription();
  static vec<VkVertexInputAttributeDescription> getAttributeDescriptions();

  bool operator==(Vertex const &other) const;
};

namespace std {
template <> struct hash<Vertex> {
  size_t operator()(Vertex const &vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >>
            1) ^
           (hash<glm::vec2>()(vertex.texCoord) << 1);
  }
};
} // namespace std

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

class App {
private:
  bool mDebugMode = true;

  shared<Core::SDL::Registry> mSDLRegistry = nullptr;
  // shared<Core::SDL::EventHandler> mSDLEventHandler = nullptr;
  shared<Core::Vulkan::Instance> mInstance = nullptr;
  shared<Core::Vulkan::DefaultDebugger> mDebugger = nullptr;
  shared<Core::SDL::Window> mWindow = nullptr;
  shared<Core::Vulkan::Device> mDevice = nullptr;
  shared<Core::Vulkan::Queue> mQueue = nullptr;

  shared<Core::Vulkan::Swapchain> mSwapchain = nullptr;

  shared<Core::Vulkan::CommandPool> mCommandPool = nullptr;
  vec<shared<Core::Vulkan::CommandBufferEncoder>> mCommandBufferEncoders;

  VkRenderPass mRenderPass = VK_NULL_HANDLE;
  VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
  VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
  VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;
  vec<VkFramebuffer> mSwapchainFramebuffers;

  u32 mMipLevels = 1;
  shared<Core::Vulkan::Image> mTextureImage = nullptr;
  shared<Core::Vulkan::ImageView> mTextureImageView = nullptr;
  VkSampler mTextureSampler = VK_NULL_HANDLE;

  shared<Core::Vulkan::Allocator> mAllocator = nullptr;

  shared<Core::Vulkan::Image> mColorImage = nullptr;
  shared<Core::Vulkan::ImageView> mColorImageView = nullptr;

  shared<Core::Vulkan::Image> mDepthImage = nullptr;
  shared<Core::Vulkan::ImageView> mDepthImageView = nullptr;

  vec<Vertex> mVertices;
  vec<u32> mIndices;
  VkBuffer mVertexBuffer = VK_NULL_HANDLE;
  VkDeviceMemory mVertexBufferMemory = VK_NULL_HANDLE;
  VkBuffer mIndexBuffer = VK_NULL_HANDLE;
  VkDeviceMemory mIndexBufferMemory = VK_NULL_HANDLE;

  vec<VkBuffer> mUniformBuffers;
  vec<VkDeviceMemory> mUniformBuffersMemory;
  vec<void *> mUniformBuffersMapped;

  VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
  vec<VkDescriptorSet> mDescriptorSets;

  vec<VkSemaphore> mImageAvailableSemaphores;
  vec<VkSemaphore> mRenderFinishedSemaphores;
  vec<VkFence> mInFlightFences;

  u32 mCurrentFrame = 0;
  bool mFramebufferResized = false;

private:
  void initWindow(int const &width, int const &height, str const &title);

  VkShaderModule createShaderModule(vec<char> const &code);

  void createCommandBuffers();

  VkFormat findSupportedFormat(vec<VkFormat> const &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat findDepthFormat();
  bool hasStencilComponent(VkFormat format);

  void createRenderPass();
  void createDescriptorSetLayout();
  void createGraphicsPipeline();
  void createColorResources();
  void createDepthResources();
  void createFramebuffers();

  u32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties);
  void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage,
                    VkMemoryPropertyFlags properties, VkBuffer &buffer,
                    VkDeviceMemory &bufferMemory);
  shared<Core::Vulkan::CommandBufferEncoder> beginSingleTimeCommands();
  void endSingleTimeCommands(
      shared<Core::Vulkan::CommandBufferEncoder> const &encoder);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout,
                             u32 mipLevels);
  void copyBufferToImage(VkBuffer buffer, VkImage image, u32 width, u32 height);
  void generateMipmaps(shared<Core::Vulkan::Image> const &image,
                       VkFormat imageFormat, i32 texWidth, i32 texHeight,
                       u32 mipLevels);

  void createTexture();
  void createTextureImageView();
  void createTextureSampler();

  void loadModel();

  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffers();

  void createDescriptorPool();
  void createDescriptorSets();

  void
  recordCommandBuffer(shared<Core::Vulkan::CommandBufferEncoder> const &encoder,
                      u32 imageIndex);

  void createSyncObjects();

  void cleanupSwapchain();
  void recreateSwapchain();

  void initVulkan();
  bool pollEvents();
  void updateUniformBuffer(u32 currentImage);
  void drawFrame();
  void mainLoop();
  void cleanup();

public:
  App(int const &width = 1500, int const &height = 750,
      str const &title = "Vulkan Tutorial", bool debugMode = true);
  ~App();

  void run();
};

int main();
