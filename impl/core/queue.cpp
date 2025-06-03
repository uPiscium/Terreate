#include "../../include/common/exception.hpp"
#include "../../include/core/queue.hpp"

namespace Terreate::Core {
Type::vec<VkSubmitInfo> GraphicQueue::createSubmitInfos() {
  Type::vec<VkSubmitInfo> submitInfos;
  submitInfos.reserve(mSubmitInfos.size());

  for (auto const &info : mSubmitInfos) {
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount =
        static_cast<Type::u32>(info.commandBuffers.size());
    submitInfo.pCommandBuffers = info.commandBuffers.data();
    submitInfo.waitSemaphoreCount =
        static_cast<Type::u32>(info.waitSemaphores.size());
    submitInfo.pWaitSemaphores = info.waitSemaphores.data();
    submitInfo.pWaitDstStageMask = info.waitStages.data();
    submitInfo.signalSemaphoreCount =
        static_cast<Type::u32>(info.signalSemaphores.size());
    submitInfo.pSignalSemaphores = info.signalSemaphores.data();

    submitInfos.push_back(submitInfo);
  }

  return submitInfos;
}

GraphicQueue::GraphicQueue(VkObjectRef<Device> device) : mDevice(device) {
  if (!mDevice) {
    throw Exception::NullReferenceException("Device is null.");
  }

  auto queue = mDevice->getQueue();
  if (!queue.graphicsFamily.has_value()) {
    throw Exception::InvalidArgument("Graphics queue family is not set.");
  }
  vkGetDeviceQueue(*mDevice, queue.graphicsFamily.value(), 0, &mQueue);
  if (mQueue == VK_NULL_HANDLE) {
    throw Exception::QueueRetrievalFailure(
        "Failed to retrieve graphics queue from the device.");
  }
}

void GraphicQueue::queue(Type::vec<VkObjectRef<CommandBuffer>> commandBuffers,
                         Type::vec<Type::PipelineStage> waitStages,
                         Type::vec<VkObjectRef<Semaphore>> waitSemaphores,
                         Type::vec<VkObjectRef<Semaphore>> signalSemaphores) {
  if (commandBuffers.size() < 1) {
    throw Exception::InvalidArgument("Command buffer is null.");
  }

  SubmitInfo submitInfo;
  submitInfo.commandBuffers.reserve(commandBuffers.size());
  for (auto const &buffer : commandBuffers) {
    if (!buffer) {
      throw Exception::InvalidArgument("Command buffer is null.");
    }
    submitInfo.commandBuffers.push_back(*buffer);
  }

  submitInfo.waitStages.reserve(waitStages.size());
  for (auto const &stage : waitStages) {
    submitInfo.waitStages.push_back(static_cast<VkPipelineStageFlags>(stage));
  }

  submitInfo.waitSemaphores.reserve(waitSemaphores.size());
  for (auto const &semaphore : waitSemaphores) {
    if (!semaphore) {
      throw Exception::InvalidArgument("Wait semaphore is null.");
    }
    submitInfo.waitSemaphores.push_back(*semaphore);
  }

  submitInfo.signalSemaphores.reserve(signalSemaphores.size());
  for (auto const &semaphore : signalSemaphores) {
    if (!semaphore) {
      throw Exception::InvalidArgument("Signal semaphore is null.");
    }
    submitInfo.signalSemaphores.push_back(*semaphore);
  }

  mSubmitInfos.push_back(submitInfo);
}

void GraphicQueue::submit(VkObjectRef<Fence> fence) {
  if (mSubmitInfos.empty()) {
    throw Exception::InvalidArgument("No submit info to submit.");
  }

  VkFence fenceVk = VK_NULL_HANDLE;
  if (fence) {
    fenceVk = *fence;
  }

  Type::vec<VkSubmitInfo> submitInfos = this->createSubmitInfos();
  VkResult result =
      vkQueueSubmit(mQueue, submitInfos.size(), submitInfos.data(), fenceVk);
  if (result != VK_SUCCESS) {
    throw Exception::QueueSubmitFailure(
        "Failed to submit command buffers to the queue.");
  }

  mSubmitInfos.clear();
}

void GraphicQueue::dispose() {
  if (mQueue != VK_NULL_HANDLE) {
    vkQueueWaitIdle(mQueue);
    mSubmitInfos.clear();
    mQueue = VK_NULL_HANDLE;
  }
}

PresentQueue::PresentQueue(VkObjectRef<Device> device) : mDevice(device) {
  if (!mDevice) {
    throw Exception::NullReferenceException("Device is null.");
  }

  auto queue = mDevice->getQueue();
  if (!queue.presentFamily.has_value()) {
    throw Exception::InvalidArgument("Present queue family is not set.");
  }
  vkGetDeviceQueue(*mDevice, queue.presentFamily.value(), 0, &mQueue);
  if (mQueue == VK_NULL_HANDLE) {
    throw Exception::QueueRetrievalFailure(
        "Failed to retrieve present queue from the device.");
  }
}

void PresentQueue::present(VkObjectRef<Swapchain> swapchain,
                           Type::vec<Type::u32> imageIndices,
                           VkObjectRef<Semaphore> waitSemaphore) {
  if (!swapchain) {
    throw Exception::InvalidArgument("Swapchain is null.");
  }

  if (imageIndices.empty()) {
    throw Exception::InvalidArgument("Image indices are empty.");
  }

  Type::vec<VkSwapchainKHR> swapchains;
  swapchains.push_back(*swapchain);
  Type::vec<VkSemaphore> waitSemaphores;
  if (waitSemaphore) {
    waitSemaphores.push_back(*waitSemaphore);
  }

  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  presentInfo.waitSemaphoreCount =
      static_cast<Type::u32>(waitSemaphores.size());
  presentInfo.pWaitSemaphores = waitSemaphores.data();
  presentInfo.swapchainCount = static_cast<Type::u32>(swapchains.size());
  presentInfo.pSwapchains = swapchains.data();
  presentInfo.pImageIndices = imageIndices.data();
  presentInfo.pResults = nullptr; // Optional, can be null if not needed

  VkResult result = vkQueuePresentKHR(mQueue, &presentInfo);
}

void PresentQueue::dispose() {
  if (mQueue != VK_NULL_HANDLE) {
    vkQueueWaitIdle(mQueue);
    mQueue = VK_NULL_HANDLE;
  }
}
} // namespace Terreate::Core
