#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vulkan/vulkan.hpp>

#include <array>
#include <chrono>
#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

typedef std::string str;
typedef int8_t i8;
typedef uint8_t u8;
typedef int16_t i16;
typedef uint16_t u16;
typedef int32_t i32;
typedef uint32_t u32;
typedef int64_t i64;
typedef uint64_t u64;

template <typename T, size_t N> using array = std::array<T, N>;
template <typename T> using vec = std::vector<T>;
template <typename K, typename V> using umap = std::unordered_map<K, V>;
template <typename T> using uset = std::unordered_set<T>;

static constexpr u32 MAX_FRAMES_IN_FLIGHT = 2;
static constexpr char const *ENGINE_NAME = "Terreate";
static constexpr int WINDOW_WIDTH = 800;
static constexpr int WINDOW_HEIGHT = 600;
static constexpr u32 ENGINE_VERSION = VK_MAKE_VERSION(0, 1, 0);
static constexpr char const *MODEL_PATH =
    "assets/models/viking_room/viking_room.obj";
static constexpr char const *TEXTURE_PATH =
    "assets/models/viking_room/viking_room.png";

static constexpr char const *VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"};
static constexpr char const *DEVICE_EXTENSIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  glm::vec2 texCoord;

  static VkVertexInputBindingDescription getBindingDescription();
  static array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions();

  bool operator==(Vertex const &other) const {
    return pos == other.pos && color == other.color &&
           texCoord == other.texCoord;
  }
};

namespace std {
template <> struct hash<Vertex> {
  size_t operator()(Vertex const &vertex) const {
    return ((hash<glm::vec3>()(vertex.pos) ^
             (hash<glm::vec3>()(vertex.color) << 1)) >>
            (hash<glm::vec2>()(vertex.texCoord) << 1));
  }
};
} // namespace std

struct UniformBufferObject {
  alignas(16) glm::mat4 model;
  alignas(16) glm::mat4 view;
  alignas(16) glm::mat4 proj;
};

struct QueueFamilyIndices {
  i32 graphicsFamily = -1;
  i32 presentFamily = -1;

  bool isComplete() const { return graphicsFamily * presentFamily >= 0; }
  operator uset<i32>() const;
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR capabilities;
  vec<VkSurfaceFormatKHR> formats;
  vec<VkPresentModeKHR> presentModes;
};

class VulkanTutorial {
private:
  bool mDebugMode = false;

  VkInstance mInstance = VK_NULL_HANDLE;
  VkDebugUtilsMessengerEXT mDebugMessenger = VK_NULL_HANDLE;

  VkSampleCountFlagBits mMSAASamples = VK_SAMPLE_COUNT_1_BIT;
  VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice mDevice = VK_NULL_HANDLE;

  VkQueue mGraphicsQueue = VK_NULL_HANDLE;
  VkQueue mPresentQueue = VK_NULL_HANDLE;

  VkSurfaceKHR mSurface = VK_NULL_HANDLE;

  VkSwapchainKHR mSwapchain = VK_NULL_HANDLE;
  vec<VkImage> mSwapchainImages;
  VkFormat mSwapchainImageFormat;
  VkExtent2D mSwapchainExtent;
  vec<VkImageView> mSwapchainImageViews;

  VkRenderPass mRenderPass = VK_NULL_HANDLE;
  VkDescriptorSetLayout mDescriptorSetLayout = VK_NULL_HANDLE;
  VkPipelineLayout mPipelineLayout = VK_NULL_HANDLE;
  VkPipeline mGraphicsPipeline = VK_NULL_HANDLE;
  vec<VkFramebuffer> mSwapchainFramebuffers;

  VkCommandPool mCommandPool = VK_NULL_HANDLE;

  VkImage mColorImage = VK_NULL_HANDLE;
  VkDeviceMemory mColorImageMemory = VK_NULL_HANDLE;
  VkImageView mColorImageView = VK_NULL_HANDLE;

  u32 mMipLevels = 1;
  VkImage mTexture = VK_NULL_HANDLE;
  VkDeviceMemory mTextureMemory = VK_NULL_HANDLE;
  VkImageView mTextureImageView = VK_NULL_HANDLE;
  VkSampler mTextureSampler = VK_NULL_HANDLE;

  VkImage mDepthImage = VK_NULL_HANDLE;
  VkDeviceMemory mDepthImageMemory = VK_NULL_HANDLE;
  VkImageView mDepthImageView = VK_NULL_HANDLE;

  vec<Vertex> mVertices;
  vec<u32> mIndices;
  VkBuffer mVertexBuffer = VK_NULL_HANDLE;
  VkDeviceMemory mVertexBufferMemory = VK_NULL_HANDLE;
  VkBuffer mIndexBuffer = VK_NULL_HANDLE;
  VkDeviceMemory mIndexBufferMemory = VK_NULL_HANDLE;

  vec<VkBuffer> mUniformBuffers;
  vec<VkDeviceMemory> mUniformBuffersMemory;
  vec<void *> mUniformBuffersMapped;

  vec<VkBuffer> mShaderStorageBuffers;
  vec<VkDeviceMemory> mShaderStorageBuffersMemory;
  vec<void *> mShaderStorageBuffersMapped;

  VkDescriptorPool mDescriptorPool = VK_NULL_HANDLE;
  vec<VkDescriptorSet> mDescriptorSets;

  vec<VkCommandBuffer> mCommandBuffers = {};

  vec<VkSemaphore> mImageAvailableSemaphores = {};
  vec<VkSemaphore> mRenderFinishedSemaphores = {};
  vec<VkFence> mInFlightFences = {};

  u32 mCurrentFrame = 0;
  bool mFramebufferResized = false;

  SDL_Window *mWindow = nullptr;

private:
  void initWindow();
  bool checkValidationLayerSupport();
  vec<char const *> getRequiredExtensions();
  void populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT &createInfo);
  void createInstance(str const &appName, u32 const &appVersion);
  VkResult createDebugUtilsMessengerEXT(
      VkDebugUtilsMessengerCreateInfoEXT const *pCreateInfo,
      VkAllocationCallbacks const *pAllocator);
  VkResult
  destroyDebugUtilsMessengerEXT(VkAllocationCallbacks const *pAllocator);
  void setupDebugMessenger();
  void createSurface();
  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
  bool checkDeviceExtensionSupport(VkPhysicalDevice device);
  SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
  int rateDevice(VkPhysicalDevice device);
  VkSampleCountFlagBits getMaxUsableSampleCount(VkPhysicalDevice device);
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createQueue();
  VkSurfaceFormatKHR
  chooseSwapSurfaceFormat(vec<VkSurfaceFormatKHR> const &availableFormats);
  VkPresentModeKHR
  chooseSwapPresentMode(vec<VkPresentModeKHR> const &availablePresentModes);
  VkExtent2D chooseSwapExtent(VkSurfaceCapabilitiesKHR const &capabilities);
  void createSwapchain();
  VkImageView createImageView(VkImage image, VkFormat format,
                              VkImageAspectFlags aspectFlags, u32 mipLevels);
  void createImageViews();
  VkShaderModule createShaderModule(vec<char> const &code);
  void createCommandPool();
  VkFormat findSupportedFormat(vec<VkFormat> const &candidates,
                               VkImageTiling tiling,
                               VkFormatFeatureFlags features);
  VkFormat findDepthFormat();
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
  VkCommandBuffer beginSingleTimeCommands();
  void endSingleTimeCommands(VkCommandBuffer commandBuffer);
  void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
  void createImage(u32 width, u32 height, u32 mipLevels,
                   VkSampleCountFlagBits samples, VkFormat format,
                   VkImageTiling tiling, VkImageUsageFlags usage,
                   VkMemoryPropertyFlags properties, VkImage &image,
                   VkDeviceMemory &imageMemory);
  bool hasStencilComponent(VkFormat format);
  void transitionImageLayout(VkImage image, VkFormat format,
                             VkImageLayout oldLayout, VkImageLayout newLayout,
                             u32 mipLevels);
  void copyBufferToImage(VkBuffer buffer, VkImage image, u32 width, u32 height);
  void generateMipmaps(VkImage image, VkFormat imageFormat, i32 texWidth,
                       i32 texHeight, u32 mipLevels);
  void createTexture();
  void createTextureImageView();
  void createTextureSampler();
  void loadModel();
  void createVertexBuffer();
  void createIndexBuffer();
  void createUniformBuffers();
  void createDescriptorPool();
  void createDescriptorSets();
  void createCommandBuffers();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, u32 imageIndex);
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
  static VKAPI_ATTR VkBool32 VKAPI_CALL
  debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                VkDebugUtilsMessengerCallbackDataEXT const *pCallbackData,
                void *pUserData);
  static vec<char> readFile(str const &filename);

public:
  VulkanTutorial(bool debug = false) : mDebugMode(debug) {}
  void run();
};

int main();
