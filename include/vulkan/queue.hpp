#pragma once
#include "command.hpp"
#include "device.hpp"
#include "sync.hpp"
#include "vkobj.hpp"

#include "../common/type.hpp"

namespace Terreate::Vulkan {
class GraphicQueue {
private:
  PROHIBIT_COPY_AND_ASSIGN(GraphicQueue);

private:
  struct SubmitInfo {
    Type::vec<VkCommandBuffer> commandBuffers;
    Type::vec<VkPipelineStageFlags> waitStages;
    Type::vec<VkSemaphore> waitSemaphores;
    Type::vec<VkSemaphore> signalSemaphores;
  };

private:
  VkObjectRef<Device> mDevice;
  VkQueue mQueue = VK_NULL_HANDLE;
  Type::vec<SubmitInfo> mSubmitInfos;

private:
  Type::vec<VkSubmitInfo> createSubmitInfos();

public:
  GraphicQueue(VkObjectRef<Device> device);
  ~GraphicQueue();

  void queue(Type::vec<VkObjectRef<CommandBuffer>> commandBuffers,
             Type::vec<Type::PipelineStage> waitStages = {},
             Type::vec<VkObjectRef<Semaphore>> waitSemaphores = {},
             Type::vec<VkObjectRef<Semaphore>> signalSemaphores = {});
  void submit(VkObjectRef<Fence> fence = nullptr);

  operator VkQueue() const { return mQueue; }
};

class PresentQueue {
private:
  PROHIBIT_COPY_AND_ASSIGN(PresentQueue);

private:
  VkObjectRef<Device> mDevice;
  VkQueue mQueue = VK_NULL_HANDLE;

public:
  PresentQueue(VkObjectRef<Device> device);
  ~PresentQueue();

  void present(VkObjectRef<Swapchain> swapchain,
               Type::vec<Type::u32> imageIndices,
               VkObjectRef<Semaphore> waitSemaphore = nullptr);

  operator VkQueue() const { return mQueue; }
};
} // namespace Terreate::Vulkan
