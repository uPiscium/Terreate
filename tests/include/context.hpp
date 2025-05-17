// #pragma once
// #define GLFW_INCLUDE_NONE
// #define GLFW_INCLUDE_VULKAN
// #include <glfw/glfw3.h>

// #include <exception.hpp>
// #include <hubptr.hpp>
// #include <instance.hpp>
// #include <type.hpp>

// namespace Terreate::Core {
// using namespace Terreate::Type;
// using namespace Terreate::Util::Pointer;

// class TerreateContext {
// private:
//   vec<HubPointer<VulkanInstance>> mInstances;

// public:
//   TerreateContext();
//   ~TerreateContext();

//   RefPointer<VulkanInstance>
//   createInstance(str const &appName, Version appVersion,
//                  str const &engineName = "No Engine",
//                  Version engineVersion = {1, 0, 0},
//                  u32 apiVersion = VK_API_VERSION_1_4);
// };
// } // namespace Terreate::Core
