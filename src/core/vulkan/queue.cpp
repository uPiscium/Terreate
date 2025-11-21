#include "core/vulkan/queue.hpp"

namespace Terreate::Vulkan {

Queue::Queue(shared<Device> const &device) : mDevice(device) {
  umap<QueueType, u32> queueFamilyIndices = mDevice->getQueueFamilyIndices();
  vkGetDeviceQueue(*mDevice, queueFamilyIndices[QueueType::GRAPHICS], 0,
                   &mHandle);
}

void Queue::submit(vec<VkSemaphore> const &wait, vec<VkSemaphore> const &signal,
                   vec<shared<CommandBuffer>> const &commandBuffer,
                   VkFence fence) const {
  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  vec<VkCommandBuffer> vkCommandBuffers;
  for (auto const &cmdBuf : commandBuffer) {
    vkCommandBuffers.push_back(*cmdBuf);
  }

  VkPipelineStageFlags waitStages[] = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
  submitInfo.waitSemaphoreCount = static_cast<u32>(wait.size());
  submitInfo.pWaitSemaphores = wait.data();
  submitInfo.pWaitDstStageMask = waitStages;

  submitInfo.commandBufferCount = static_cast<u32>(commandBuffer.size());
  submitInfo.pCommandBuffers = vkCommandBuffers.data();

  submitInfo.signalSemaphoreCount = static_cast<u32>(signal.size());
  submitInfo.pSignalSemaphores = signal.data();

  if (vkQueueSubmit(mHandle, 1, &submitInfo, fence) != VK_SUCCESS) {
    throw QueueException("Failed to submit draw command buffer to queue.");
  }
}

void Queue::submit(vec<shared<CommandBuffer>> const &commandBuffer,
                   VkFence fence) const {
  this->submit({}, {}, commandBuffer, fence);
}

vec<bool> Queue::present(vec<shared<Swapchain>> swapchains, u32 imageIndex,
                         vec<VkSemaphore> const &wait) const {
  vec<VkResult> results(swapchains.size());
  vec<VkSwapchainKHR> vkSwapchains;
  for (auto const &swapchain : swapchains) {
    vkSwapchains.push_back(*swapchain);
  }
  VkPresentInfoKHR presentInfo{};
  presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

  presentInfo.waitSemaphoreCount = static_cast<u32>(wait.size());
  presentInfo.pWaitSemaphores = wait.data();

  presentInfo.swapchainCount = static_cast<u32>(vkSwapchains.size());
  presentInfo.pSwapchains = vkSwapchains.data();
  presentInfo.pImageIndices = &imageIndex;
  presentInfo.pResults = results.data();

  vkQueuePresentKHR(mHandle, &presentInfo);

  vec<bool> status;
  for (auto const &result : results) {
    if (result == VK_SUBOPTIMAL_KHR || result == VK_ERROR_OUT_OF_DATE_KHR) {
      status.push_back(false);
    } else if (result == VK_SUCCESS) {
      status.push_back(true);
    } else {
      throw QueueException("Failed to present swap chain image.");
    }
  }

  return status;
}

bool Queue::present(shared<Swapchain> const &swapchain, u32 imageIndex,
                    vec<VkSemaphore> const &wait) const {
  vec<shared<Swapchain>> swapchains = {swapchain};
  vec<bool> status = this->present(swapchains, imageIndex, wait);
  return status[0];
}

void Queue::wait() const { vkQueueWaitIdle(mHandle); }

Queue::operator VkQueue() const { return mHandle; }

shared<Queue> Queue::create(shared<Device> const &device) {
  Queue *queue = new Queue(device);
  return shared<Queue>(queue);
}

} // namespace Terreate::Vulkan
