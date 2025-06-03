// #include <debugger.hpp>

// namespace Terreate::Core {
// DebugObject::DebugObject(VkDebugUtilsObjectNameInfoEXT const &info)
//     : objectType(info.objectType), handle(info.objectHandle) {
//   if (info.pObjectName) {
//     name = info.pObjectName;
//   } else {
//     name = "Unknown";
//   }
// }

// VKAPI_ATTR VkBool32 VKAPI_CALL
// debugCallbackWrapper(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//                      VkDebugUtilsMessageTypeFlagsEXT messageType,
//                      VkDebugUtilsMessengerCallbackDataEXT const
//                      *pCallbackData, void *pUserData) {
//   if (!pUserData) {
//     return VK_FALSE;
//   }

//   IDebugger *debugger = static_cast<IDebugger *>(pUserData);
//   bool result = false;
//   vec<DebugObject> debugObjects(pCallbackData->pObjects,
//                                 pCallbackData->pObjects +
//                                     pCallbackData->objectCount);
//   switch (messageSeverity) {
//   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
//     result = debugger->verbose(pCallbackData->pMessage,
//                                (MessageType)messageType, debugObjects);
//     break;
//   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
//     result = debugger->info(pCallbackData->pMessage,
//     (MessageType)messageType,
//                             debugObjects);
//     break;
//   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
//     result = debugger->warning(pCallbackData->pMessage,
//                                (MessageType)messageType, debugObjects);
//     break;
//   case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
//     result = debugger->error(pCallbackData->pMessage,
//     (MessageType)messageType,
//                              debugObjects);
//     break;
//   default:
//     result = false; // Unknown severity is not an error.
//     break;
//   }
//   return static_cast<VkBool32>(result);
// }

// } // namespace Terreate::Core
