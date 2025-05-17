// #pragma once

// #define GLFW_INCLUDE_NONE
// #define GLFW_INCLUDE_VULKAN
// #include <glfw/glfw3.h>

// #include <api.hpp>
// #include <debugger.hpp>
// #include <device.hpp>
// #include <hubptr.hpp>
// #include <type.hpp>
// #include <window.hpp>

// namespace Terreate::Core {
// using namespace Terreate::Type;
// using namespace Terreate::Util::Pointer;

// class VulkanInstance {
// private:
//   VkInstance mInstance;
//   VkDebugUtilsMessengerCreateInfoEXT createInfo;
//   VkDebugUtilsMessengerEXT mDebugMessenger;
//   vec<HubPointer<Window>> mWindows;
//   HubPointer<LogicalDevice> mLogicalDevice;

// private:
//   VkDebugUtilsMessengerCreateInfoEXT createDebugCreateInfo() const;

// public:
//   VulkanInstance();
//   VulkanInstance(str const &appName, Version appVersion, str const
//   &engineName,
//                  Version engineVersion, u32 apiVersion);
//   ~VulkanInstance();

//   VkInstance getInstance() const { return mInstance; } // TODO: remove

//   void attachDebugger(IDebugger *debugger);

//   RefPointer<Window> createWindow(str const &title, u32 width, u32 height,
//                                   WindowSettings settings =
//                                   WindowSettings{});
// };
// } // namespace Terreate::Core
